cmd_arch/arm/boot/dts/imx53-smd.dtb := mkdir -p arch/arm/boot/dts/ ; gcc -E -Wp,-MD,arch/arm/boot/dts/.imx53-smd.dtb.d.pre.tmp -nostdinc -I./scripts/dtc/include-prefixes -undef -D__DTS__ -x assembler-with-cpp -o arch/arm/boot/dts/.imx53-smd.dtb.dts.tmp arch/arm/boot/dts/imx53-smd.dts ; ./scripts/dtc/dtc -O dtb -o arch/arm/boot/dts/imx53-smd.dtb -b 0 -iarch/arm/boot/dts/ -i./scripts/dtc/include-prefixes -Wno-unit_address_vs_reg -Wno-unit_address_format -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -Wno-graph_child_address -Wno-simple_bus_reg -Wno-unique_unit_address -Wno-pci_device_reg  -d arch/arm/boot/dts/.imx53-smd.dtb.d.dtc.tmp arch/arm/boot/dts/.imx53-smd.dtb.dts.tmp ; cat arch/arm/boot/dts/.imx53-smd.dtb.d.pre.tmp arch/arm/boot/dts/.imx53-smd.dtb.d.dtc.tmp > arch/arm/boot/dts/.imx53-smd.dtb.d

source_arch/arm/boot/dts/imx53-smd.dtb := arch/arm/boot/dts/imx53-smd.dts

deps_arch/arm/boot/dts/imx53-smd.dtb := \
  scripts/dtc/include-prefixes/dt-bindings/input/input.h \
  scripts/dtc/include-prefixes/dt-bindings/input/linux-event-codes.h \
  arch/arm/boot/dts/imx53.dtsi \
  arch/arm/boot/dts/imx53-pinfunc.h \
  scripts/dtc/include-prefixes/dt-bindings/clock/imx5-clock.h \
  scripts/dtc/include-prefixes/dt-bindings/gpio/gpio.h \
  scripts/dtc/include-prefixes/dt-bindings/interrupt-controller/irq.h \

arch/arm/boot/dts/imx53-smd.dtb: $(deps_arch/arm/boot/dts/imx53-smd.dtb)

$(deps_arch/arm/boot/dts/imx53-smd.dtb):
