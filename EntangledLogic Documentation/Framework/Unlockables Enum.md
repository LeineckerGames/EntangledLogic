The Unlockables Enum stores the definition for `EUnlockables`.

## Unlockables
***
The Unlockables are assigned in the [[Item Data Structs|Factory Data Table]] and are used to specify unlockable requirement is needed to unlock a factory.

`EUnlockables` stores the values of:
- **Unlocked**
- **Factory_X_Gate**
- **Factory_Y_Gate**
- **Factory_Z_Gate**
- **Factory_H_Gate**
- **Factory_S_Gate**
- **Factory_T_Gate**
- **Factory_CNOT_Gate**
- **Factory_TierTwoProgression**
- **Factory_QuantumGnome**
- **Dev_Testing_Factories**
- **Test_RainbowBaseGridColor**
- **UnlockAll**

>[!note] Unlock All
>The `UnlockAll` value needs to stay as the last value in the enum. This is done so when using the [[Testing Factories|Developer Factory]] to easily iterate through the enum when unlocking all factories.