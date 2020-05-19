cmd_arch/arm/boot/dts/imx6ul-geam.dtb := mkdir -p arch/arm/boot/dts/ ; gcc -E -Wp,-MD,arch/arm/boot/dts/.imx6ul-geam.dtb.d.pre.tmp -nostdinc -I./scripts/dtc/include-prefixes -undef -D__DTS__ -x assembler-with-cpp -o arch/arm/boot/dts/.imx6ul-geam.dtb.dts.tmp arch/arm/boot/dts/imx6ul-geam.dts ; ./scripts/dtc/dtc -O dtb -o arch/arm/boot/dts/imx6ul-geam.dtb -b 0 -iarch/arm/boot/dts/ -i./scripts/dtc/include-prefixes -Wno-unit_address_vs_reg -Wno-unit_address_format -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -Wno-graph_child_address -Wno-simple_bus_reg -Wno-unique_unit_address -Wno-pci_device_reg  -d arch/arm/boot/dts/.imx6ul-geam.dtb.d.dtc.tmp arch/arm/boot/dts/.imx6ul-geam.dtb.dts.tmp ; cat arch/arm/boot/dts/.imx6ul-geam.dtb.d.pre.tmp arch/arm/boot/dts/.imx6ul-geam.dtb.d.dtc.tmp > arch/arm/boot/dts/.imx6ul-geam.dtb.d

source_arch/arm/boot/dts/imx6ul-geam.dtb := arch/arm/boot/dts/imx6ul-geam.dts

deps_arch/arm/boot/dts/imx6ul-geam.dtb := \
  scripts/dtc/include-prefixes/dt-bindings/gpio/gpio.h \
  scripts/dtc/include-prefixes/dt-bindings/input/input.h \
  scripts/dtc/include-prefixes/dt-bindings/input/linux-event-codes.h \
  arch/arm/boot/dts/imx6ul.dtsi \
  scripts/dtc/include-prefixes/dt-bindings/clock/imx6ul-clock.h \
  scripts/dtc/include-prefixes/dt-bindings/interrupt-controller/arm-gic.h \
  scripts/dtc/include-prefixes/dt-bindings/interrupt-controller/irq.h \
  arch/arm/boot/dts/imx6ul-pinfunc.h \

arch/arm/boot/dts/imx6ul-geam.dtb: $(deps_arch/arm/boot/dts/imx6ul-geam.dtb)

$(deps_arch/arm/boot/dts/imx6ul-geam.dtb):
