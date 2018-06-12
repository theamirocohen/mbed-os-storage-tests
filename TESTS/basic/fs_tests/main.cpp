/* Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "LittleFileSystem.h"

#ifndef TEST_SD 
#define TEST_SPIF
#endif

#if !defined(TEST_SPIF) && !defined(TEST_SD)
#error [NOT_SUPPORTED] storage test not supported on this platform
#endif

#ifdef TEST_SPIF
#include "SPIFBlockDevice.h"
#elif defined TEST_SD
#include "SDBlockDevice.h"
#endif

using namespace utest::v1;

static const size_t buf_size    = 10;
static const size_t test_files  = 4;

FILE *fd[test_files];

#ifdef TEST_SPIF
SPIFBlockDevice bd(
    MBED_CONF_SPIF_DRIVER_SPI_MOSI, 
    MBED_CONF_SPIF_DRIVER_SPI_MISO,  
    MBED_CONF_SPIF_DRIVER_SPI_CLK, 
    MBED_CONF_SPIF_DRIVER_SPI_CS
    );
#elif defined TEST_SD
SDBlockDevice bd(
    MBED_CONF_SD_SPI_MOSI, 
    MBED_CONF_SD_SPI_MISO, 
    MBED_CONF_SD_SPI_CLK, 
    MBED_CONF_SD_SPI_CS
    );
#endif

/*----------------help functions------------------*/

static void init()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);
}

static void deinit()
{
    int res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------fopen()------------------*/

//fopen path without fs prefix
static void FS_fopen_path_not_valid()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(1, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fopen empty file name with r mode
static void FS_fopen_empty_path_r_mode()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "", "rb")) != NULL);
    TEST_ASSERT_EQUAL(1, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fopen empty file name with w mode
static void FS_fopen_empty_path_w_mode()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "", "wb")) != NULL);
    TEST_ASSERT_EQUAL(1, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fopen empty mode
static void FS_fopen_invalid_mode()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "Invalid_mode", "")) != NULL);
    TEST_ASSERT_EQUAL(1, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);
    
    deinit();
}

//fopen with valid flow
static void FS_fopen_supported_wb_plus_mode()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

/*----------------fclose()------------------*/

//fclose valid flow
static void FS_fclose_valid_flow()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

/*----------------fwrite()------------------*/

//fwrite with nmemb zero
static void FS_fwrite_nmemb_zero()
{
    char buffer[buf_size] = "good_day";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), 0, fd[0]);
    TEST_ASSERT_EQUAL(0, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);  

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fwrite valid flow
static void FS_fwrite_valid_flow()
{
    char buffer[buf_size] = "good_day";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fwrite to fopen mode r
static void FS_fwrite_with_fopen_r_mode()
{
    char buffer[buf_size] = "good_day";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);
    
    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

/*----------------fread()------------------*/

//fread with size zero
static void FS_fread_size_zero()
{
    char buffer[buf_size] = "good_day";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(buffer, 0, buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fread with nmemb zero
static void FS_fread_nmemb_zero()
{
    char buffer[buf_size] = "good_day";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(buffer, sizeof(char), 0, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fread valid flow
static void FS_fread_valid_flow()
{
    char buffer[buf_size] = "good_day", rbuffer[buf_size] = {};

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(rbuffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, read_sz);
    TEST_ASSERT_EQUAL_STRING(buffer, rbuffer);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fread to fopen mode w
static void FS_fread_with_fopen_w_mode()
{
    char buffer[buf_size] = {};

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

/*----------------setup------------------*/

Case cases[] = {
    Case("FS_fopen_path_not_valid", FS_fopen_path_not_valid),
    Case("FS_fopen_empty_path_r_mode", FS_fopen_empty_path_r_mode),
    Case("FS_fopen_empty_path_w_mode", FS_fopen_empty_path_w_mode),
    Case("FS_fopen_invalid_mode", FS_fopen_invalid_mode),
    Case("FS_fopen_supported_wb_plus_mode", FS_fopen_supported_wb_plus_mode),

    Case("FS_fclose_valid_flow", FS_fclose_valid_flow),

    Case("FS_fwrite_nmemb_zero", FS_fwrite_nmemb_zero),
    Case("FS_fwrite_valid_flow", FS_fwrite_valid_flow),
    Case("FS_fwrite_with_fopen_r_mode", FS_fwrite_with_fopen_r_mode),
    
    Case("FS_fread_size_zero", FS_fread_size_zero),
    Case("FS_fread_nmemb_zero", FS_fread_nmemb_zero),
    Case("FS_fread_valid_flow", FS_fread_valid_flow),
    Case("FS_fread_with_fopen_w_mode", FS_fread_with_fopen_w_mode),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    return !Harness::run(specification);
}
