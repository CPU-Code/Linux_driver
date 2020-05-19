cmd_arch/arm/boot/dts/imx7d-colibri-emmc-eval-v3.dtb := mkdir -p arch/arm/boot/dts/ ; gcc -E -Wp,-MD,arch/arm/boot/dts/.imx7d-colibri-emmc-eval-v3.dtb.d.pre.tmp -nostdinc -I./scripts/dtc/include-prefixes -undef -D__DTS__ -x assembler-with-cpp -o arch/arm/boot/dts/.imx7d-colibri-emmc-eval-v3.dtb.dts.tmp arch/arm/boot/dts/imx7d-colibri-emmc-eval-v3.dts ; ./scripts/dtc/dtc -O dtb -o arch/arm/boot/dts/imx7d-colibri-emmc-eval-v3.dtb -b 0 -iarch/arm/boot/dts/ -i./scripts/dtc/include-prefixes -Wno-unit_address_vs_reg -Wno-unit_address_format -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -Wno-graph_child_address -Wno-simple_bus_reg -Wno-unique_unit_address -Wno-pci_device_reg  -d arch/arm/boot/dts/.imx7d-colibri-emmc-eval-v3.dtb.d.dtc.tmp arch/arm/boot/dts/.imx7d-colibri-emmc-eval-v3.dtb.dts.tmp ; cat arch/arm/boot/dts/.imx7d-colibri-emmc-eval-v3.dtb.d.pre.tmp arch/arm/boot/dts/.imx7d-colibri-emmc-eval-v3.dtb.d.dtc.tmp > arch/arm/boot/dts/.imx7d-colibri-emmc-eval-v3.dtb.d

source_arch/arm/boot/dts/imx7d-colibri-emmc-eval-v3.dtb := arch/arm/boot/dts/imx7d-colibri-emmc-eval-v3.dts

deps_arch/arm/boot/dts/imx7d-colibri-emmc-eval-v3.dtb := \
  arch/arm/boot/dts/imx7d-colibri-emmc.dtsi \
  arch/arm/boot/dts/imx7d.dtsi \
  arch/arm/boot/dts/imx7s.dtsi \
  scripts/dtc/include-prefixes/dt-bindings/clock/imx7d-clock.h \
  scripts/dtc/include-prefixes/dt-bindings/power/imx7-power.h \
  scripts/dtc/include-prefixes/dt-bindings/gpio/gpio.h \
  scripts/dtc/include-prefixes/dt-bindings/input/input.h \
  scripts/dtc/include-prefixes/dt-bindings/input/linux-event-codes.h \
  scripts/dtc/include-prefixes/dt-bindings/interrupt-controller/arm-gic.h \
  scripts/dtc/include-prefixes/dt-bindings/interrupt-controller/irq.h \
  scripts/dtc/include-prefixes/dt-bindings/reset/imx7-reset.h \
  arch/arm/boot/dts/imx7d-pinfunc.h \
  arch/arm/boot/dts/imx7-colibri.dtsi \
  arch/arm/boot/dts/imx7-colibri-eval-v3.dtsi \

arch/arm/boot/dts/imx7d-colibri-emmc-eval-v3.dtb: $(deps_arch/arm/boot/dts/imx7d-colibri-emmc-eval-v3.dtb)

$(deps_arch/arm/boot/dts/imx7d-colibri-emmc-eval-v3.dtb):
