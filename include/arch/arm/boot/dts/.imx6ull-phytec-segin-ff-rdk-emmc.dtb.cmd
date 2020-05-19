cmd_arch/arm/boot/dts/imx6ull-phytec-segin-ff-rdk-emmc.dtb := mkdir -p arch/arm/boot/dts/ ; gcc -E -Wp,-MD,arch/arm/boot/dts/.imx6ull-phytec-segin-ff-rdk-emmc.dtb.d.pre.tmp -nostdinc -I./scripts/dtc/include-prefixes -undef -D__DTS__ -x assembler-with-cpp -o arch/arm/boot/dts/.imx6ull-phytec-segin-ff-rdk-emmc.dtb.dts.tmp arch/arm/boot/dts/imx6ull-phytec-segin-ff-rdk-emmc.dts ; ./scripts/dtc/dtc -O dtb -o arch/arm/boot/dts/imx6ull-phytec-segin-ff-rdk-emmc.dtb -b 0 -iarch/arm/boot/dts/ -i./scripts/dtc/include-prefixes -Wno-unit_address_vs_reg -Wno-unit_address_format -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -Wno-graph_child_address -Wno-simple_bus_reg -Wno-unique_unit_address -Wno-pci_device_reg  -d arch/arm/boot/dts/.imx6ull-phytec-segin-ff-rdk-emmc.dtb.d.dtc.tmp arch/arm/boot/dts/.imx6ull-phytec-segin-ff-rdk-emmc.dtb.dts.tmp ; cat arch/arm/boot/dts/.imx6ull-phytec-segin-ff-rdk-emmc.dtb.d.pre.tmp arch/arm/boot/dts/.imx6ull-phytec-segin-ff-rdk-emmc.dtb.d.dtc.tmp > arch/arm/boot/dts/.imx6ull-phytec-segin-ff-rdk-emmc.dtb.d

source_arch/arm/boot/dts/imx6ull-phytec-segin-ff-rdk-emmc.dtb := arch/arm/boot/dts/imx6ull-phytec-segin-ff-rdk-emmc.dts

deps_arch/arm/boot/dts/imx6ull-phytec-segin-ff-rdk-emmc.dtb := \
  arch/arm/boot/dts/imx6ull.dtsi \
  arch/arm/boot/dts/imx6ul.dtsi \
  scripts/dtc/include-prefixes/dt-bindings/clock/imx6ul-clock.h \
  scripts/dtc/include-prefixes/dt-bindings/gpio/gpio.h \
  scripts/dtc/include-prefixes/dt-bindings/input/input.h \
  scripts/dtc/include-prefixes/dt-bindings/input/linux-event-codes.h \
  scripts/dtc/include-prefixes/dt-bindings/interrupt-controller/arm-gic.h \
  scripts/dtc/include-prefixes/dt-bindings/interrupt-controller/irq.h \
  arch/arm/boot/dts/imx6ul-pinfunc.h \
  arch/arm/boot/dts/imx6ull-pinfunc.h \
  arch/arm/boot/dts/imx6ull-pinfunc-snvs.h \
  arch/arm/boot/dts/imx6ull-phytec-phycore-som.dtsi \
  arch/arm/boot/dts/imx6ul-phytec-phycore-som.dtsi \
  scripts/dtc/include-prefixes/dt-bindings/pwm/pwm.h \
  arch/arm/boot/dts/imx6ull-phytec-segin.dtsi \
  arch/arm/boot/dts/imx6ul-phytec-segin.dtsi \
  arch/arm/boot/dts/imx6ull-phytec-segin-peb-eval-01.dtsi \
  arch/arm/boot/dts/imx6ul-phytec-segin-peb-eval-01.dtsi \

arch/arm/boot/dts/imx6ull-phytec-segin-ff-rdk-emmc.dtb: $(deps_arch/arm/boot/dts/imx6ull-phytec-segin-ff-rdk-emmc.dtb)

$(deps_arch/arm/boot/dts/imx6ull-phytec-segin-ff-rdk-emmc.dtb):
