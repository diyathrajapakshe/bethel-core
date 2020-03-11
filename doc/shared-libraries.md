Shared Libraries
================

## bethelconsensus

The purpose of this library is to make the verification functionality that is critical to Bethel's consensus available to other applications, e.g. to language bindings.

### API

The interface is defined in the C header `bethelconsensus.h` located in  `src/script/bethelconsensus.h`.

#### Version

`bethelconsensus_version` returns an `unsigned int` with the API version *(currently at an experimental `0`)*.

#### Script Validation

`bethelconsensus_verify_script` returns an `int` with the status of the verification. It will be `1` if the input script correctly spends the previous output `scriptPubKey`.

##### Parameters
- `const unsigned char *scriptPubKey` - The previous output script that encumbers spending.
- `unsigned int scriptPubKeyLen` - The number of bytes for the `scriptPubKey`.
- `const unsigned char *txTo` - The transaction with the input that is spending the previous output.
- `unsigned int txToLen` - The number of bytes for the `txTo`.
- `unsigned int nIn` - The index of the input in `txTo` that spends the `scriptPubKey`.
- `unsigned int flags` - The script validation flags *(see below)*.
- `bethelconsensus_error* err` - Will have the error/success code for the operation *(see below)*.

##### Script Flags
- `bethelconsensus_SCRIPT_FLAGS_VERIFY_NONE`
- `bethelconsensus_SCRIPT_FLAGS_VERIFY_P2SH` - Evaluate P2SH ([BIP16](https://github.com/bethel/bips/blob/master/bip-0016.mediawiki)) subscripts
- `bethelconsensus_SCRIPT_FLAGS_VERIFY_DERSIG` - Enforce strict DER ([BIP66](https://github.com/bethel/bips/blob/master/bip-0066.mediawiki)) compliance

##### Errors
- `bethelconsensus_ERR_OK` - No errors with input parameters *(see the return value of `bethelconsensus_verify_script` for the verification status)*
- `bethelconsensus_ERR_TX_INDEX` - An invalid index for `txTo`
- `bethelconsensus_ERR_TX_SIZE_MISMATCH` - `txToLen` did not match with the size of `txTo`
- `bethelconsensus_ERR_DESERIALIZE` - An error deserializing `txTo`

### Example Implementations
- [NBethel](https://github.com/NicolasDorier/NBethel/blob/master/NBethel/Script.cs#L814) (.NET Bindings)
- [node-libbethelconsensus](https://github.com/bitpay/node-libbethelconsensus) (Node.js Bindings)
- [java-libbethelconsensus](https://github.com/dexX7/java-libbethelconsensus) (Java Bindings)
- [bethelconsensus-php](https://github.com/Bit-Wasp/bethelconsensus-php) (PHP Bindings)
