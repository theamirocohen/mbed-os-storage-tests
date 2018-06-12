# mbed-os-storage-test

FileSystem tests for Mbed OS

## Getting started with storage FS tests ##

This is the general tests of FS APIs.

The application invokes the POSIX APIs for opening, reading, writing and closing files, the POSIX APIs is using the FS APIs to complete the tasks and prints the results after each such invocation. 

These tests can be performed on any device with SPIF or SD, choosing between them is defined in compile time by adding the -D option to 'mbed test' command (specific examples are below), when no -D option added the test will assume SPIF test is running.

The SPIF and SD block devices gets their values automatically from their own mbed_lib.json file, the files will be visible after 'mbed deploy', for SPIF the file is at the spif-driver root directory, for SD the file is at the sd-driver/config directory.

## Required hardware
In our example we will use K82F development board:
* An [FRDM-K82F](http://os.mbed.com/platforms/FRDM-K82F/) development board.
* A micro-USB cable.

### Other hardware
Although the board shown in this example is K82F, the example should work on any Mbed enabled hardware with external memory.

##  Getting started ##

 1. Import the repository.

    ```
    mbed mbed-os-storage-test
    cd mbed-os-storage-test
    ```

 2. Compile and generate tests.

    For example, for `GCC` with `K82F` and `SPIF` (SPIF is the default option so -DTEST_SPIF can be omitted):

    ```
    mbed test -m K82F -t GCC_ARM -n tests-basic-fs_tests -DTEST_SPIF --compile
    ```

    Or for `GCC` with `K64F` and `SD`:

    ```
    mbed test -m K64F -t GCC_ARM -n tests-basic-fs_tests -DTEST_SD --compile
    ```

 3. Run test.

    For example, for `GCC` with `K82`:

    ```
    mbed test -m K82F -t GCC_ARM -n tests-basic-fs_tests --run -v
    ```

## Troubleshooting

If you have problems, you can review the [documentation](https://os.mbed.com/docs/latest/tutorials/debugging.html) for suggestions on what could be wrong and how to fix it.
