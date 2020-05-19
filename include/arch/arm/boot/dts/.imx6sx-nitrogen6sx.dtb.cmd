cmd_arch/arm/boot/dts/imx6sx-nitrogen6sx.dtb := mkdir -p arch/arm/boot/dts/ ; gcc -E -Wp,-MD,arch/arm/boot/dts/.imx6sx-nitrogen6sx.dtb.d.pre.tmp -nostdinc -I./scripts/dtc/include-prefixes -undef -D__DTS__ -x assembler-with-cpp -o arch/arm/boot/dts/.imx6sx-nitrogen6sx.dtb.dts.tmp arch/arm/boot/dts/imx6sx-nitrogen6sx.dts ; ./scripts/dtc/dtc -O dtb -o arch/arm/boot/dts/imx6sx-nitrogen6sx.dtb -b 0 -iarch/arm/boot/dts/ -i./scripts/dtc/include-prefixes -Wno-unit_address_vs_reg -Wno-unit_address_format -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -Wno-graph_child_address -Wno-simple_bus_reg -Wno-unique_unit_address -Wno-pci_device_reg  -d arch/arm/boot/dts/.imx6sx-nitrogen6sx.dtb.d.dtc.tmp arch/arm/boot/dts/.imx6sx-nitrogen6sx.dtb.dts.tmp ; cat arch/arm/boot/dts/.imx6sx-nitrogen6sx.dtb.d.pre.tmp arch/arm/boot/dts/.imx6sx-nitrogen6sx.dtb.d.dtc.tmp > arch/arm/boot/dts/.imx6sx-nitrogen6sx.dtb.d

source_arch/arm/boot/dts/imx6sx-nitrogen6sx.dtb := arch/arm/boot/dts/imx6sx-nitrogen6sx.dts

deps_arch/arm/boot/dts/imx6sx-nitrogen6sx.dtb := \
  arch/arm/boot/dts/imx6sx.dtsi \
  scripts/dtc/include-prefixes/dt-bindings/clock/imx6sx-clock.h \
  scripts/dtc/include-prefixes/dt-bindings/gpio/gpio.h \
  scripts/dtc/include-prefixes/dt-bindings/input/input.h \
  scripts/dtc/include-prefixes/dt-bindings/input/linux-event-codes.h \
  scripts/dtc/include-prefixes/dt-bindings/interrupt-controller/arm-gic.h \
  scripts/dtc/include-prefixes/dt-bindings/interrupt-controller/irq.h \
  arch/arm/boot/dts/imx6sx-pinfunc.h \

arch/arm/boot/dts/imx6sx-nitrogen6sx.dtb: $(deps_arch/arm/boot/dts/imx6sx-nitrogen6sx.dtb)

$(deps_arch/arm/boot/dts/imx6sx-nitrogen6sx.dtb):
