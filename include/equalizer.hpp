#ifndef EQUALIZER_HPP
#define EQUALIZER_HPP

#include "plugin_base.hpp"

class Equalizer : public PluginBase {
 public:
  Equalizer(const std::string& tag,
            const std::string& schema,
            const std::string& schema_left,
            const std::string& schema_right);
  Equalizer(const Equalizer&) = delete;
  auto operator=(const Equalizer&) -> Equalizer& = delete;
  Equalizer(const Equalizer&&) = delete;
  auto operator=(const Equalizer &&) -> Equalizer& = delete;
  ~Equalizer() override;

  GstElement* equalizer = nullptr;

  void update_equalizer();

 private:
  GSettings *settings_left = nullptr, *settings_right = nullptr;

  void bind_band(GstElement* equalizer, const int& index);
};

#endif
