#include "source_output_effects_ui.hpp"

SourceOutputEffectsUi::SourceOutputEffectsUi(
    BaseObjectType* cobject,
    const Glib::RefPtr<Gtk::Builder>& refBuilder,
    std::shared_ptr<SourceOutputEffects> soe_ptr)
    : EffectsBaseUi(cobject, refBuilder, soe_ptr->pm),
      soe(soe_ptr),
      settings(
          Gio::Settings::create("com.github.wwmm.pulseeffects.sourceoutputs")),
      limiter_ui(LimiterUi::create(
          "com.github.wwmm.pulseeffects.sourceoutputs.limiter")),
      compressor_ui(CompressorUi::create(
          "com.github.wwmm.pulseeffects.sourceoutputs.compressor")) {
    // limiter level meters connections

    connections.push_back(soe->limiter->input_level.connect(
        sigc::mem_fun(*limiter_ui, &LimiterUi::on_new_input_level)));
    connections.push_back(soe->limiter->output_level.connect(
        sigc::mem_fun(*limiter_ui, &LimiterUi::on_new_output_level)));
    connections.push_back(soe->limiter->attenuation.connect(
        sigc::mem_fun(*limiter_ui, &LimiterUi::on_new_attenuation)));

    // compressor level meters connections

    connections.push_back(soe->compressor_input_level.connect(
        sigc::mem_fun(*compressor_ui, &CompressorUi::on_new_input_level_db)));
    connections.push_back(soe->compressor_output_level.connect(
        sigc::mem_fun(*compressor_ui, &CompressorUi::on_new_output_level_db)));
    connections.push_back(soe->compressor->compression.connect(
        sigc::mem_fun(*compressor_ui, &CompressorUi::on_new_compression)));

    add_plugins();
}

SourceOutputEffectsUi::~SourceOutputEffectsUi() {
    for (auto c : connections) {
        c.disconnect();
    }
}

std::unique_ptr<SourceOutputEffectsUi> SourceOutputEffectsUi::create(
    std::shared_ptr<SourceOutputEffects> soe) {
    auto builder = Gtk::Builder::create_from_resource(
        "/com/github/wwmm/pulseeffects/effects_base.glade");

    SourceOutputEffectsUi* soe_ui = nullptr;

    builder->get_widget_derived("widgets_box", soe_ui, soe);

    return std::unique_ptr<SourceOutputEffectsUi>(soe_ui);
}

void SourceOutputEffectsUi::add_plugins() {
    auto plugins_order = Glib::Variant<std::vector<std::string>>();

    settings->get_value("plugins", plugins_order);

    for (auto name : plugins_order.get()) {
        if (name == std::string("limiter")) {
            add_to_listbox(limiter_ui);
            stack->add(*limiter_ui, std::string("limiter"));
        } else if (name == std::string("compressor")) {
            add_to_listbox(compressor_ui);
            stack->add(*compressor_ui, std::string("compressor"));
        }
    }
}

void SourceOutputEffectsUi::reset() {}