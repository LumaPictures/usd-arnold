# USD-Arnold
USD Schemas and tools for exchanging Arnold shader information between multiple 3rd party packages.

## Project Goals
* Have a centralized set of schemas describing Arnold materials.
* Provide writers and readers for rendering applications for seamlessly transferring shader information from one application to another.

## Modules
### Current 
* usdAi schema. 
    * AiNodeApi - Handling generic Arnold nodes.
    * AiShapeAPI - Handling Arnold shape options, like visibility, sidedness, self-shadows, and receive-shadows.
    * AiMaterialAPI - Defining Arnold shader relationships.
    * AiProcedural - Schema for Arnold's procedural node.
    * AiVolume - Schema for Arnold's volume node.
* Shader exporter for usdMaya. A custom shading mode exporter for Maya that exports all Arnold shader definitions via MtoA. We support MtoA-1.2 and MtoA-1.4.
* Tools for usdKatana. Ops for describing and reading in procedurals to Katana.

### Planned
* Supporting MtoA-2.x.
* Adding a material importer for usdKatana. For more information, see building USD.
* Making sure USD-Arnold works with a base installation of USD.
* Build all the packages at once.
* Add windows support.

## Building

Our build process is designed to build USD-Arnold as multiple rez packages, so the different components need to be built separately from the main schema.

At this moment USD-Arnold relies on several customizations and fixes to USD, most of which have their PR and waiting to be merged back.

### Use our custom build of USD
Building Luma Pictures' flavor of USD is the easiest option, we provide a set of new features, mostly to usdMaya and usdKatana, none of which are breaking any existing behavior in USD. Clone the repository at [https://github.com/LumaPictures/USD](https://github.com/LumaPictures/USD) and build the branch tg/luma/luma following the official build instructions. Note, we changed the default implementation for TfHashMap and TfHashSet to improve stability, make sure you build all your plugins using this version.

### Use the official version and apply patches
Clone the latest dev version of USD, and apply the following change to support importing arnold materials in Katana:

Replace the file with Luma's [version](https://github.com/LumaPictures/USD/blob/tg/luma/luma/third_party/katana/lib/usdKatana/readMaterial.cpp).

## Build enviroment
### Requirements

We use and test with the following library versions.

| Name    | Version          |
| ------- | ---------------- |
| GCC     | 4.8.5            |
| Boost   | 1.61.0           |
| TBB     | 4.4.6            |
| OpenEXR | 2.2.0 - latest   |
| MtoA    | 1.2.x and 1.4.2  |
| Katana  | 2.5v6            |
| Maya    | 2016.5 and 2017  |

You can use configuration variables to enable/disable individual components during the build process. These are the following.
* BUILD\_USD\_PLUGIN - Generating the schemas.
* BUILD\_USD\_IMAGING\_PLUGIN - Building the render delegate.
* BUILD\_USD\_MAYA\_PLUGIN - Building the usdMaya plugin.
* BUILD\_USD\_KATANA\_PLUGIN - Building the usdKatana plugin.

TODO: Finish.

## Contributing

TODO.
