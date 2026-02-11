# Prospector Dongle Setup Plan for Corne Keyboard

**Date:** February 11, 2026  
**Target:** Convert Corne split keyboard to use Prospector dongle as central controller  
**Current Branch:** prospector

---

## 📊 Current Setup Analysis

### Hardware Configuration
- **Left Half:** nice!nano v2 + nice!view display (currently central)
- **Right Half:** nice!nano v2 + nice!view display (currently peripheral)
- **Target Dongle:** Seeeduino XIAO BLE + Prospector adapter with OLED

### Software Configuration
- **Layout:** Standard Corne 3x6+3 (42 keys total)
- **Layers:** 4 layers with display names already defined
  - Layer 0: "Colemak" (default)
  - Layer 1: "Num" (numbers and navigation)
  - Layer 2: "Qwerty" (alternative layout)
  - Layer 3: "BT" (Bluetooth controls)
- **Current Features:**
  - ZMK Studio enabled on left half (will be REMOVED)
  - nice!view displays on both halves
  - Custom home row mods using zmk-helpers

---

## 🎯 Target Architecture

```
                    ┌──────────────────────┐
                    │  Prospector Dongle   │
                    │  (Seeeduino XIAO)    │ ← Central Controller
                    │  - OLED Display      │ ← USB to PC
                    │  - Shows layer name  │
                    └──────────┬───────────┘
                               │
                         BLE Split
                    ┌──────────┴──────────┐
                    │                     │
            ┌───────▼────────┐    ┌──────▼────────┐
            │  Corne Left    │    │  Corne Right  │
            │  (Peripheral)  │    │  (Peripheral) │
            │  nice!view     │    │  nice!view    │
            └────────────────┘    └───────────────┘
```

**Key Changes:**
- Dongle becomes the central (connects to PC)
- Both left and right halves become peripherals
- Dongle has no physical keys (mock kscan)
- Dongle has matrix transform matching the split halves
- All three devices communicate via BLE

---

## 📝 Implementation Steps

### **Step 1: Update West Manifest**
**File:** `config/west.yml`

**Action:** Add prospector-zmk-module as a dependency

**Changes:**
```yaml
manifest:
  remotes:
    - name: zmkfirmware
      url-base: https://github.com/zmkfirmware
    - name: urob
      url-base: https://github.com/urob
    - name: carrefinho                            # ← ADD THIS
      url-base: https://github.com/carrefinho     # ← ADD THIS
  projects:
    - name: zmk
      remote: zmkfirmware
      revision: main
      import: app/west.yml
    - name: zmk-helpers
      remote: urob
      revision: main
    - name: prospector-zmk-module                 # ← ADD THIS
      remote: carrefinho                          # ← ADD THIS
      revision: main                              # ← ADD THIS
  self:
    path: config
```

**Why:** Provides the prospector adapter shield and display driver code

---

### **Step 2: Create Shield Directory Structure**
**Action:** Create directory for custom Corne shield overrides

**Commands:**
```powershell
New-Item -ItemType Directory -Path "c:\Users\Jarrett\zmk-config\config\boards\shields\corne" -Force
```

**Why:** Corne shields are built into ZMK, but we need to override them to add dongle support. ZMK will prioritize local shield definitions over built-in ones.

---

### **Step 3: Create Kconfig.shield**
**File:** `config/boards/shields/corne/Kconfig.shield`

**Content:**
```kconfig
config SHIELD_CORNE_LEFT
    def_bool $(shields_list_contains,corne_left)

config SHIELD_CORNE_RIGHT
    def_bool $(shields_list_contains,corne_right)

config SHIELD_CORNE_DONGLE
    def_bool $(shields_list_contains,corne_dongle)
```

**Why:** Defines the three shield variants (left, right, dongle) so ZMK build system recognizes them

---

### **Step 4: Create Kconfig.defconfig**
**File:** `config/boards/shields/corne/Kconfig.defconfig`

**Content:**
```kconfig
if SHIELD_CORNE_LEFT

config ZMK_KEYBOARD_NAME
    default "Corne"

config ZMK_SPLIT_ROLE_CENTRAL
    default n

endif

if SHIELD_CORNE_RIGHT

config ZMK_KEYBOARD_NAME
    default "Corne"

config ZMK_SPLIT_ROLE_CENTRAL
    default n

endif

if SHIELD_CORNE_DONGLE

config ZMK_KEYBOARD_NAME
    default "Corne"

config ZMK_SPLIT_ROLE_CENTRAL
    default y

config ZMK_SPLIT
    default y

# Set to number of peripherals (left + right = 2)
config ZMK_SPLIT_BLE_CENTRAL_PERIPHERALS
    default 2

# BT_MAX_CONN = ZMK_SPLIT_BLE_CENTRAL_PERIPHERALS + BT profiles (2 + 5 = 7)
config BT_MAX_CONN
    default 7

# Set to same as BT_MAX_CONN
config BT_MAX_PAIRED
    default 7

endif
```

**Why:** 
- Forces left and right to be peripherals (not central)
- Configures dongle as central controller
- Sets up Bluetooth connections for 2 peripherals + 5 profiles

**Note:** The default ZMK Corne config makes left side central. This overrides that behavior.

---

### **Step 5: Create Dongle Overlay**
**File:** `config/boards/shields/corne/corne_dongle.overlay`

**Content:**
```dts
#include <dt-bindings/zmk/matrix_transform.h>

/ {
    chosen {
        zmk,kscan = &mock_kscan;
        zmk,matrix_transform = &default_transform;
    };

    mock_kscan: mock_kscan_0 {
        compatible = "zmk,kscan-mock";
        columns = <0>;
        rows = <0>;
        events = <0>;
    };

    default_transform: keymap_transform_0 {
        compatible = "zmk,matrix-transform";
        columns = <12>;
        rows = <4>;
        
        // Corne 3x6+3 layout
        // ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
        // │ 0 │ 1 │ 2 │ 3 │ 4 │ 5 │       │ 6 │ 7 │ 8 │ 9 │10 │11 │
        // ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
        // │12 │13 │14 │15 │16 │17 │       │18 │19 │20 │21 │22 │23 │
        // ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
        // │24 │25 │26 │27 │28 │29 │       │30 │31 │32 │33 │34 │35 │
        // └───┴───┴───┼───┼───┼───┤       ├───┼───┼───┼───┴───┴───┘
        //             │36 │37 │38 │       │39 │40 │41 │
        //             └───┴───┴───┘       └───┴───┴───┘
        
        map = <
            RC(0,0) RC(0,1) RC(0,2) RC(0,3) RC(0,4) RC(0,5)  RC(0,6) RC(0,7) RC(0,8) RC(0,9) RC(0,10) RC(0,11)
            RC(1,0) RC(1,1) RC(1,2) RC(1,3) RC(1,4) RC(1,5)  RC(1,6) RC(1,7) RC(1,8) RC(1,9) RC(1,10) RC(1,11)
            RC(2,0) RC(2,1) RC(2,2) RC(2,3) RC(2,4) RC(2,5)  RC(2,6) RC(2,7) RC(2,8) RC(2,9) RC(2,10) RC(2,11)
                            RC(3,3) RC(3,4) RC(3,5)  RC(3,6) RC(3,7) RC(3,8)
        >;
    };
};
```

**Why:**
- Dongle has no physical keys, so we use a mock kscan (no actual scanning)
- Matrix transform must exactly match the split halves' layout
- Defines the 42-key Corne layout (3x6 + 3 thumbs per side)

**Important:** This matrix must match the keymap bindings exactly!

---

### **Step 6: Create/Update Dongle Configuration**
**File:** `config/corne.conf`

**Action:** Update existing file to add dongle-specific settings

**Add to file:**
```properties
# Prospector Dongle Configuration
CONFIG_PROSPECTOR_USE_AMBIENT_LIGHT_SENSOR=n
CONFIG_PROSPECTOR_FIXED_BRIGHTNESS=80
```

**Why:** 
- Prospector may not have ambient light sensor built-in
- Sets fixed brightness to 80 (range 0-100)
- These settings only apply when building the dongle shield

**Alternative:** Could create `config/boards/shields/corne/corne_dongle.conf` for dongle-only settings

---

### **Step 7: Update Build Configuration**
**File:** `build.yaml`

**Current:**
```yaml
include:
  - board: nice_nano_v2
    shield: corne_left nice_view_adapter nice_view
    snippet: studio-rpc-usb-uart
    cmake-args: -DCONFIG_ZMK_STUDIO=y
  - board: nice_nano_v2
    shield: corne_right nice_view_adapter nice_view
```

**New:**
```yaml
include:
  # Left peripheral (no longer central, Studio removed)
  - board: nice_nano_v2
    shield: corne_left nice_view_adapter nice_view
  
  # Right peripheral
  - board: nice_nano_v2
    shield: corne_right nice_view_adapter nice_view
  
  # Dongle central with prospector display
  - board: seeeduino_xiao_ble
    shield: corne_dongle prospector_adapter
  
  # Settings reset firmware (flash after main firmware)
  - board: nice_nano_v2
    shield: settings_reset
  - board: seeeduino_xiao_ble
    shield: settings_reset
```

**Changes:**
1. **Removed ZMK Studio** from left half (snippet and cmake-args)
2. **Left half** is now just peripheral (Kconfig.defconfig handles role)
3. **Added dongle build** with seeeduino_xiao_ble board + prospector_adapter
4. **Added settings_reset** builds for all board types

**Why:**
- Both halves become simple peripherals
- Dongle becomes the central controller
- Settings reset required after flashing new split configuration

---

### **Step 8: Update Keymap (Layer Labels)**
**File:** `config/corne.keymap`

**Action:** Verify layer labels are present (they already are!)

**Current layer labels:**
```c
default_layer {
    display-name = "Colemak";  // ✅ Already defined
    ...
}

layer_1 {
    display-name = "Num";      // ✅ Already defined
    ...
}

layer_2 {
    display-name = "Qwerty";   // ✅ Already defined
    ...
}

layer_3 {
    display-name = "BT";       // ✅ Already defined
    ...
}
```

**Action Required:** Remove `&studio_unlock` binding from BT layer

**Find this:**
```c
layer_3 {
    display-name = "BT";
    bindings = <
        ...
        &trans      &to 0         &trans        &studio_unlock        &trans  &trans
        ...
    >;
};
```

**Replace with:**
```c
layer_3 {
    display-name = "BT";
    bindings = <
        ...
        &trans      &to 0         &trans        &trans        &trans  &trans
        ...
    >;
};
```

**Why:** 
- Layer labels will display on prospector OLED
- Studio is being removed, so studio_unlock binding will cause build errors

---

### **Step 9: Commit and Push**
**Commands:**
```powershell
cd c:\Users\Jarrett\zmk-config
git add -A
git commit -m "Add Prospector dongle support to Corne - remove Studio"
git push origin prospector
```

**Why:** Triggers GitHub Actions build workflow

---

### **Step 10: Monitor Build**
**Action:** Check GitHub Actions at https://github.com/jarrettq/zmk-config/actions

**Expected Outputs:**
1. `corne_left-nice_nano_v2-zmk.uf2` (peripheral)
2. `corne_right-nice_nano_v2-zmk.uf2` (peripheral)
3. `corne_dongle-seeeduino_xiao_ble-zmk.uf2` (central)
4. `settings_reset-nice_nano_v2.uf2`
5. `settings_reset-seeeduino_xiao_ble.uf2`

---

## 🔧 Flashing Procedure

### Critical: Flash in this exact order!

#### **Step 1: Settings Reset (ALL devices)**
Flash settings reset firmware to clear old BLE pairings:

1. **Left half:** Flash `settings_reset-nice_nano_v2.uf2`
2. **Right half:** Flash `settings_reset-nice_nano_v2.uf2`
3. **Dongle:** Flash `settings_reset-seeeduino_xiao_ble.uf2`

**Important:** 
- Power off all devices between flashing
- Don't have multiple devices powered on simultaneously during reset
- Wait for drive to eject after each flash

#### **Step 2: Flash Main Firmware**
Flash the new ZMK firmware:

1. **Left half:** Flash `corne_left-nice_nano_v2-zmk.uf2`
2. **Right half:** Flash `corne_right-nice_nano_v2-zmk.uf2`
3. **Dongle:** Flash `corne_dongle-seeeduino_xiao_ble-zmk.uf2`

#### **Step 3: Pairing**
1. Power on **dongle** first (connect to PC via USB)
2. Power on **left half** (should auto-pair with dongle)
3. Power on **right half** (should auto-pair with dongle)
4. Prospector OLED should show "Corne" and current layer
5. Test all keys - both halves should work

---

## 🐛 Troubleshooting

### Build Fails

**Error:** `zmk,underglow chosen node must be declared`
- **Cause:** RGB underglow enabled but no LED hardware defined
- **Fix:** Disable in `config/corne.conf`: comment out `CONFIG_ZMK_RGB_UNDERGLOW=y`

**Error:** `'studio_unlock' undeclared`
- **Cause:** Studio removed but binding still in keymap
- **Fix:** Replace `&studio_unlock` with `&trans` in keymap

**Error:** `shield 'corne_dongle' not found`
- **Cause:** Missing shield files or incorrect directory structure
- **Fix:** Verify `config/boards/shields/corne/` contains all files

### Pairing Issues

**Symptom:** Halves don't connect to dongle
- **Fix:** Re-flash settings reset on all devices
- **Fix:** Ensure dongle is powered first
- **Fix:** Check BT profiles on dongle (should show 2 connections)

**Symptom:** Keys don't work
- **Fix:** Verify USB cable is data cable, not charge-only
- **Fix:** Check dongle is recognized by PC as USB HID device
- **Fix:** Verify both peripherals are connected (check prospector display)

### Display Issues

**Symptom:** Prospector OLED shows wrong layer
- **Fix:** Verify layer labels match keymap structure
- **Fix:** Check display-name properties are defined for all layers

**Symptom:** Prospector OLED is blank
- **Fix:** Check brightness setting in config
- **Fix:** Verify prospector_adapter shield is in build
- **Fix:** Check I2C connections on prospector hardware

---

## 📚 Reference Links

- **Prospector Module:** https://github.com/carrefinho/prospector-zmk-module
- **ZMK Dongle Guide:** https://zmk.dev/docs/development/hardware-integration/dongle
- **Tutorial Source:** https://beekeeb.com/how-to-add-dongle-and-prospector-support-to-hshs52-hshs46/
- **Prospector Hardware:** Search for "prospector zmk dongle" (designed by carrefinho)

---

## ✅ Success Criteria

When everything works correctly:

- ✅ Dongle connects to PC via USB
- ✅ Both keyboard halves connect to dongle via BLE
- ✅ All 42 keys respond to input
- ✅ Layer switching works correctly
- ✅ Prospector OLED displays current layer name
- ✅ nice!view displays on both halves still work
- ✅ Bluetooth profiles can be switched on BT layer
- ✅ Battery life unchanged on halves (still using BLE, not USB)

---

## 📋 Files Summary

### Files to Create:
1. ✅ `config/boards/shields/corne/Kconfig.shield`
2. ✅ `config/boards/shields/corne/Kconfig.defconfig`
3. ✅ `config/boards/shields/corne/corne_dongle.overlay`

### Files to Modify:
4. ✅ `config/west.yml` - Add prospector module
5. ✅ `build.yaml` - Add dongle, remove Studio
6. ✅ `config/corne.conf` - Add prospector settings
7. ✅ `config/corne.keymap` - Remove studio_unlock binding

### Expected Build Outputs:
- `corne_left-nice_nano_v2-zmk.uf2`
- `corne_right-nice_nano_v2-zmk.uf2`
- `corne_dongle-seeeduino_xiao_ble-zmk.uf2`
- `settings_reset-nice_nano_v2.uf2`
- `settings_reset-seeeduino_xiao_ble.uf2`

---

## 🚀 Ready to Execute?

Once you've reviewed this plan, we can execute it step by step. Each step builds on the previous one, so we'll go in order. Let me know when you're ready to start with **Step 1: Update West Manifest**!
