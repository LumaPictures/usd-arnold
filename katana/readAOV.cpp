#include "readAOV.h"

#include "arnoldHelpers.h"

#include <FnAttribute/FnGroupBuilder.h>

/*
    Each group below this location is named usually the same as the
    input AOV in Arnold, like indirect_diffuse etc...

    Each AOV definition has several attributes / groups.
    Attributes
        - channel -> The input AOV name.
        - type -> The typename of the AOV.
        - lightExpressions -> List of strings with the light expressions.
        - name -> The output name of the AOV (?). This seems to be the same
          as the incoming name for now.
        - driver -> The typename of the driver.
        - filter -> The typename of the filter

    Groups
        - driverParameters -> Parameters of the driver. These are the same
          as the parameters on the arnold node.
        - filterParameters -> Parameters of the filter. These are the same
          as the parameters on the arnold node.

*/
PXR_NAMESPACE_OPEN_SCOPE

namespace {
    template <typename T>
    using _attributeDefinition = AttributeDefinition<UsdAiAOV>;
}

FnKat::Attribute readAOV(const UsdAiAOV& aov) {
    FnKat::GroupBuilder builder;
    static const std::vector<_attributeDefinition<TfToken>> stringAttrs {
        {&UsdAiAOV::GetNameAttr, "name"},
        {&UsdAiAOV::GetDataTypeAttr, "subdiv_adaptive_metric"},
    };

    auto needToBuild = handleAttributes<TfToken>(stringAttrs, aov, builder);
    return needToBuild ? builder.build() : FnKat::Attribute();
}

PXR_NAMESPACE_CLOSE_SCOPE
