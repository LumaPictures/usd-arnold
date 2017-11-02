#include "readPrim.h"

#include <pxr/usd/usdAi/aiShapeAPI.h>
#include <pxr/usd/usdAi/aiMaterialAPI.h>
#include <pxr/usd/usdShade/material.h>


#include <usdKatana/attrMap.h>
#include <usdKatana/utils.h>

#include "arnoldHelpers.h"
#include "readAOV.h"
#include "readMaterial.h"

PXR_NAMESPACE_OPEN_SCOPE
/*
    There are a couple of things that are important to note when
    handling connections in Katana. We let USD import its connections
    but we are overwriting it using the read prim location function
    to make sure all the component connections work fine that we support
    in usd-arnold.

    When USD imports a shading network, it's creating a network material
    for each network material (so certain nodes might get duplicated). Since
    the network material in Katana uses strings to identify each material,
    we need to use a remapping function to figure out the names for the materials.
    This is PxrUsdKatanaUtils::GenerateShadingNodeHandle . Without this function
    you won't get the right shader names in some cases.

    Katana creates a group attribute that stores all the nodes, named material.nodes,
    which has all node definitions. Each node has multiple parameters, the most important
    for us is the connections group attribute. This stores the incoming connections
    to each node. Each sub attribute here is either a String Attribute, or a Group Attribute
    storing multiple String Attributes.

    In case of a connection where the target is the full parameter (ie, none of it's components)
    you can setup the connection by creating a string attribute like
    ("TargetParamName", "SourceParamName")

    If you have a connection that's targeting a parameter's connection, you need to create a group
    attribute named like the parameter, and a string attribute with the component's name.
    Or just create a String Attribute like parameter.component, and that'll create the group
    for you. It looks like this:
    ("TargetParamName.TargetComponentName", "SourceParamName")

    Component names are simply r, g, b, a, x, y, z in case of colors and vectors, otherwise
    you need to use the i prefix for array elements, like i0, i1, i2.

    Source connection can also represent a parameter or a parameter's component. In the case of
    a parameter, you need to name the source parameter the following way:
    out@SourceNodeName

    If you are connecting a parameter's component, you need to name the source param like this:
    out.componentName@SourceNodeName

    Like: out.r@SourceNodeName, out.x@SourceNodeName

    A couple of example String Attributes that represent a valid connection.

    ("kd_Color", "out@MyTexture")
    ("inputs:i1", "out@Checker")
    ("kd_Color.r", "out.g@MyTexture")


    AOVS

    Arnold stores the AOV settings on the root node.
    In "arnoldGlobalStatements.Channel Definitions.outputChannels"
    Note the space in the name.

*/ 
void readPrimLocation(
    FnKat::GeolibCookInterface& interface,
    FnKat::GroupAttribute opArgs,
    PxrUsdKatanaUsdInPrivateData* privateData) {
    // privateData can be a nullptr!
    if (privateData == nullptr) { return; }
    const auto prim = privateData->GetUsdPrim();
    // As of 0.8.2 this function is also called on the root: "/"
    if (!prim.IsValid() || !prim.GetPrimPath().IsPrimPath()) { return; }

    static const std::string statementsName("arnoldStatements");
        updateOrCreateAttr(interface, statementsName, getArnoldStatementsGroup(prim));

    auto stage = prim.GetStage();
    if (stage == nullptr) { return; }

    // We are handling connections here, because usd-arnold stores the connections in it's own way.
    // So we check for the materials connected to the node.
    const UsdShadeMaterial material(prim);
    if (material) {
        UsdAiMaterialAPI aiMaterialAPI(prim);
        readMaterial(stage, interface, aiMaterialAPI);
    }

    // Sadly, this is not a feasible approach.
    // I'm leaving the code here as a reference.
    /*const UsdAiAOV aov(prim);
    if (aov) {
        const auto grp = readAOV(aov);
        if (grp.isValid()) {
            // The easiest way here is to run the AttributeSet op.
            FnKat::GroupBuilder builder;
            builder.set(aov.GetPath().GetName(), grp);
            interface.extendAttr(
                "arnoldGlobalStatements.Channel Definitions.outputChannels", builder.build(),
                std::string(), true, "/root");
        }
    }*/
}

PXR_NAMESPACE_CLOSE_SCOPE
