#include "readUSDVolVolume.h"

#include <usdKatana/attrMap.h>
#include <usdKatana/readPrim.h>
#include <usdKatana/readXformable.h>

#include <pxr/usd/usdVol/openVDBAsset.h>
#include <pxr/usd/usdVol/volume.h>

#include "arnoldHelpers.h"

#include <FnAttribute/FnDataBuilder.h>

PXR_NAMESPACE_OPEN_SCOPE


void
readUSDVolVolume(
        FnKat::GeolibCookInterface& interface,
        FnKat::GroupAttribute opArgs,
        const PxrUsdKatanaUsdInPrivateData& privateData)
{
    const auto prim = privateData.GetUsdPrim();
    const auto volume = UsdVolVolume(prim);
    const auto currentTime = privateData.GetUsdInArgs()->GetCurrentTime();

    const auto fieldPathMap = volume.GetFieldPaths();
    if (fieldPathMap.empty()) {
        ReportError(interface, "UsdVolVolume has no field relationships");
        return;
    }

    const auto stagePtr = prim.GetStage();
    const auto volumePath = prim.GetPath();

    std::unordered_set<std::string> vdbPaths;
    std::vector<std::string> vdbFieldNames;
    FnKat::GroupBuilder skipChildren;

    SdfAssetPath filePath;
    TfToken fieldName;
    for (const auto& fieldPair : fieldPathMap) {
        const UsdPrim& fieldPrim = stagePtr->GetPrimAtPath(fieldPair.second);
        if (!fieldPrim) {
            continue;
        }
        const auto vdbField = UsdVolOpenVDBAsset(fieldPrim);
        if (!vdbField) {
            continue;
        }
        if (fieldPair.second.GetParentPath() == volumePath) {
            skipChildren.set(fieldPrim.GetName().GetText(), FnKat::IntAttribute(1));
        }
        vdbField.GetFilePathAttr().Get<SdfAssetPath>(&filePath, currentTime);
        if (vdbPaths.insert(filePath.GetResolvedPath()).second
                && vdbPaths.size() > 1) {
            ReportError(interface,
                        "UsdVolVolume has VDB fields with differing asset paths");
            return;
        }
        // Trust the field name attribute on the VDB asset over the relationship
        // name. Note that Houdini 17 currently writes time samples for this
        // attribute, so we just use the first available sample in that case.
        if (!vdbField.GetFieldNameAttr().Get<TfToken>(&fieldName)) {
            vdbField.GetFieldNameAttr().Get<TfToken>(&fieldName, 
                                                     UsdTimeCode::EarliestTime());
        }
        vdbFieldNames.push_back(fieldName.GetString());
    }

    interface.setAttr("type", FnKat::StringAttribute("volume"));
    
    if (vdbPaths.size() != 1) {
        TF_WARN("UsdVolVolume %s has no VDB asset fields. No Arnold volume "
                "will be configured.", volumePath.GetText());
        return;
    }

    // Read in the generic prim attributes. This handles things like material
    // binding, visibility, primvars, etc.
    PxrUsdKatanaAttrMap attrs;
    PxrUsdKatanaReadPrim(prim, privateData, attrs);
    attrs.toInterface(interface);

    // Advertise the connected field names
    interface.setAttr("info.usdAi.vdbFields", 
                      FnKat::StringAttribute(vdbFieldNames));

    // If any of the VDB field prims are children of the Volume prim (which is
    // the recommended organization), tell the `pxrUsdIn` op to skip over them,
    // to avoid generating extra empty groups that may cause confusion.
    if (skipChildren.isValid()) {
        interface.setAttr("__UsdIn.skipChild", skipChildren.build());
    }

    // Build up the arguments required to execute an ArnoldOpenVDBVolume op.
    FnKat::GroupBuilder argsBuilder;
    getArnoldVDBVolumeOpArgs(prim, argsBuilder);
    argsBuilder.set("filename", FnKat::StringAttribute(*vdbPaths.begin()));

    FnKat::GroupAttribute xform;
    if (PxrUsdKatanaReadXformable(volume, privateData, xform)) {
        argsBuilder.set("xform", xform);
    }

    // Forward the time slice information to the Arnold op so the motion range 
    // gets set up properly on the volume.
    FnKat::GroupAttribute timeSlice = interface.getOpArg("system.timeSlice");;
    argsBuilder.set("system", FnKat::GroupAttribute("timeSlice", timeSlice, false), false);

    interface.execOp("ArnoldOpenVDBVolume", argsBuilder.build());
}

PXR_NAMESPACE_CLOSE_SCOPE
