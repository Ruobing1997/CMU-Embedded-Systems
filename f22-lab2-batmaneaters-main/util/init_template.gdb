define armex
  if ($lr == -3)
    printf "Uses PSP 0x%x return.\n", $psp
    set $armex_base = $psp
  else
    printf "Uses MSP 0x%x return.\n", $msp
    set $armex_base = $msp
  end

  printf "xPSR          0x%x : 0x%x\n", (int*)($armex_base+28), *(int*)($armex_base+28)
  printf "ReturnAddress 0x%x : 0x%x\n", (int*)($armex_base+24), *(int*)($armex_base+24)
  printf "LR (R14)      0x%x : 0x%x\n", (int*)($armex_base+20), *(int*)($armex_base+20)
  printf "R12           0x%x : 0x%x\n", (int*)($armex_base+16), *(int*)($armex_base+16)
  printf "R3            0x%x : 0x%x\n", (int*)($armex_base+12), *(int*)($armex_base+12)
  printf "R2            0x%x : 0x%x\n", (int*)($armex_base+8), *(int*)($armex_base+8)
  printf "R1            0x%x : 0x%x\n", (int*)($armex_base+4), *(int*)($armex_base+4)
  printf "R0            0x%x : 0x%x\n", (int*)($armex_base), *(int*)($armex_base)
  printf "Return instruction:\n"
  x/i *(int*)($armex_base+24)
end

define examine_stack
  set $i = 0
  while $i < $arg0
    printf "stack[%d] : 0x%08x : 0x%08x\n",$i,($armex_base + $i * 4) , *(int*)($armex_base + $i * 4)
    set $i = $i + 1
  end
end

define reset
  monitor reset
end

define examine_hard_fault
  set $HFSR = *(int*)(0xE000ED2C)
  set $CFSR = *(int*)(0xE000ED28)
  if (($HFSR >> 30) & 0x1) 
    printf "Hard fault was forced\n"
  end
  if (($CFSR >> 18) & 0x1)
    printf "Invalid PC load UsageFault, caused by an invalid PC load by EXC_RETURN\n"
  end
end

set arch armv7e-m
set mem inaccessible-by-default off
file build/bin/<template-file>.elf
tar ext <template-port>
mon tpwr enable
mon swdp_scan
att 1
mon rtt enable
# lay src
load
run
