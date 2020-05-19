cmd_arch/arm/boot/dts/imx35-eukrea-mbimxsd35-baseboard.dtb := mkdir -p arch/arm/boot/dts/ ; gcc -E -Wp,-MD,arch/arm/boot/dts/.imx35-eukrea-mbimxsd35-baseboard.dtb.d.pre.tmp -nostdinc -I./scripts/dtc/include-prefixes -undef -D__DTS__ -x assembler-with-cpp -o arch/arm/boot/dts/.imx35-eukrea-mbimxsd35-baseboard.dtb.dts.tmp arch/arm/boot/dts/imx35-eukrea-mbimxsd35-baseboard.dts ; ./scripts/dtc/dtc -O dtb -o arch/arm/boot/dts/imx35-eukrea-mbimxsd35-baseboard.dtb -b 0 -iarch/arm/boot/dts/ -i./scripts/dtc/include-prefixes -Wno-unit_address_vs_reg -Wno-unit_address_format -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -Wno-graph_child_address -Wno-simple_bus_reg -Wno-unique_unit_address -Wno-pci_device_reg  -d arch/arm/boot/dts/.imx35-eukrea-mbimxsd35-baseboard.dtb.d.dtc.tmp arch/arm/boot/dts/.imx35-eukrea-mbimxsd35-baseboard.dtb.dts.tmp ; cat arch/arm/boot/dts/.imx35-eukrea-mbimxsd35-baseboard.dtb.d.pre.tmp arch/arm/boot/dts/.imx35-eukrea-mbimxsd35-baseboard.dtb.d.dtc.tmp > arch/arm/boot/dts/.imx35-eukrea-mbimxsd35-baseboard.dtb.d

source_arch/arm/boot/dts/imx35-eukrea-mbimxsd35-baseboard.dtb := arch/arm/boot/dts/imx35-eukrea-mbimxsd35-baseboard.dts

deps_arch/arm/boot/dts/imx35-eukrea-mbimxsd35-baseboard.dtb := \
  scripts/dtc/include-prefixes/dt-bindings/gpio/gpio.h \
  scripts/dtc/include-prefixes/dt-bindings/input/input.h \
  scripts/dtc/include-prefixes/dt-bindings/input/linux-event-codes.h \
  arch/arm/boot/dts/imx35-eukrea-cpuimx35.dtsi \
  arch/arm/boot/dts/imx35.dtsi \
  arch/arm/boot/dts/imx35-pinfunc.h \

arch/arm/boot/dts/imx35-eukrea-mbimxsd35-baseboard.dtb: $(deps_arch/arm/boot/dts/imx35-eukrea-mbimxsd35-baseboard.dtb)

$(deps_arch/arm/boot/dts/imx35-eukrea-mbimxsd35-baseboard.dtb):
