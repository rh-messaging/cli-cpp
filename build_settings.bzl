# https://docs.google.com/document/d/1vc8v-kXjvgZOdQdnxPTaV0rrLxtP2XwnD2tAZlYJOqw

# This file defines the build_flag and build_setting rules which can be used in
# BUILD files to declare specific settings.
BuildSettingInfo = provider(fields = ["name", "value"])

def _build_setting_impl(ctx):
  # The rule implementation. We could do validation, type conversion, etc. if we
  # want. But this example just dumps ctx.build_setting_value into a provider.
  return [BuildSettingInfo(name = ctx.attr.name, value = ctx.build_setting_value )]

string_flag = rule(
  implementation = _build_setting_impl,
  build_setting = config.string(flag=True) # Settable at the command line
)

bool_flag = rule(
  implementation = _build_setting_impl,
  build_setting = config.bool(flag=True)
)

label_flag = rule(
  implementation = _build_setting_impl,
  #build_setting = attr.label()
)
