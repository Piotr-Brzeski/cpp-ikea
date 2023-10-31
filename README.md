# cpp-ikea
Manage Ikea Tradfri from C++

## Build
Use [XcodeGen](https://github.com/yonaskolb/XcodeGen) to generate Xcode project
or [NinjaGen](https://github.com/Piotr-Brzeski/NinjaGen) to generate ninja build file.

## Examples

In order to build example application
create `configuration.h` file in the `examples` directory.
Content of the `configuration.h` file:

```
namespace configuration {

constexpr char const* ip = "127.0.0.1";
constexpr char const* identity = "name";
constexpr char const* key = "key";

}
```
