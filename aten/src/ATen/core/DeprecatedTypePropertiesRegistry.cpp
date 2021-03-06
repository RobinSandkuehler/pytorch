#include <ATen/core/DeprecatedTypePropertiesRegistry.h>

#include <ATen/core/DeprecatedTypeProperties.h>

namespace at {

void DeprecatedTypePropertiesDeleter::operator()(DeprecatedTypeProperties * ptr) {
  delete ptr;
}

DeprecatedTypePropertiesRegistry::DeprecatedTypePropertiesRegistry() {
  for (int b = 0; b < static_cast<int>(Backend::NumOptions); ++b) {
    for (int s = 0; s < static_cast<int>(ScalarType::NumOptions); ++s) {
      for (int v = 0; v < 2; ++ v) {
        registry[b][s][v] = c10::guts::make_unique<DeprecatedTypeProperties>(
                static_cast<Backend>(b),
                static_cast<ScalarType>(s),
                v);
      }
    }
  }
}

DeprecatedTypeProperties& DeprecatedTypePropertiesRegistry::getDeprecatedTypeProperties(
    Backend p, ScalarType s, bool is_variable) const {
  return *registry[static_cast<int>(p)][static_cast<int>(s)][is_variable];
}

// TODO: This could be bad juju if someone calls globalContext() in the
// destructor of an object with static lifetime.
DeprecatedTypePropertiesRegistry & globalDeprecatedTypePropertiesRegistry() {
  static DeprecatedTypePropertiesRegistry singleton;
  return singleton;
}

}
