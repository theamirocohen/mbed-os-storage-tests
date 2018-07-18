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

static const size_t small_buf_size    = 10;
static const size_t medium_buf_size   = 250;
static const size_t large_buf_size    = 1200;
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
static void FS_fopen_supported_wb_mode()
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

//fopen with append mode
static void FS_fopen_supported_a_mode()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "a")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fopen with read mode
static void FS_fopen_supported_r_mode()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "r")) != NULL);
    TEST_ASSERT_EQUAL(1, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fopen with append update mode
static void FS_fopen_supported_a_update_mode()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "a+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fopen with read update mode
static void FS_fopen_supported_r_update_mode()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "r+")) != NULL);
    TEST_ASSERT_EQUAL(1, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fopen with write update mode
static void FS_fopen_supported_w_update_mode()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "w+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fopen with read update create
static void FS_fopen_read_update_create()
{
    char write_buf[small_buf_size] = "123456789";
    char read_buf[small_buf_size] = "";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "r+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, read_sz);

    TEST_ASSERT_EQUAL_STRING_LEN(write_buf, read_buf, small_buf_size);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fopen with write update create
static void FS_fopen_write_update_create()
{
    char write_buf[small_buf_size] = "123456789";
    char read_buf[small_buf_size] = "";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "w+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
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

//fclose an already closed file
static void FS_fclose_closed_file()
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
    char buffer[small_buf_size] = "good_day";

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
    char write_buf[small_buf_size] = "good_day";
    char read_buf[small_buf_size] = {};

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fwrite to fopen mode r
static void FS_fwrite_with_fopen_r_mode()
{
    char buffer[small_buf_size] = "good_day";

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

    int write_sz = fwrite(buffer, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fwrite with stream is null
static void FS_fwrite_stream_null()
{
    char buffer[small_buf_size] = "good_day";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), small_buf_size, NULL);
    TEST_ASSERT_EQUAL(0, write_sz);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fwrite with buffer is null
static void FS_fwrite_buff_null()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(NULL, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fwrite with size is zero
static void FS_fwrite_size_zero()
{
    char buffer[small_buf_size] = "good_day";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, 0, small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

/*----------------fread()------------------*/

//fread with size zero
static void FS_fread_size_zero()
{
    char buffer[small_buf_size] = "good_day";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(buffer, 0, small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fread with nmemb zero
static void FS_fread_nmemb_zero()
{
    char buffer[small_buf_size] = "good_day";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

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

//fread to fopen mode w
static void FS_fread_with_fopen_w_mode()
{
    char buffer[small_buf_size] = {};

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

    int read_sz = fread(buffer, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fread to fopen with buffer as null
static void FS_fread_with_ptr_null()
{
    char write_buf[small_buf_size] = "123456789";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(NULL, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fread to fwrite file
static void FS_fread_to_fwrite_file()
{
    char read_buf[small_buf_size] = {};
    char write_buf[small_buf_size] = "123456789";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);

    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

    int read_sz = fread(read_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fread empty file
static void FS_fread_empty_file()
{
    char read_buf[small_buf_size] = {};

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

    int read_sz = fread(read_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fread valid flow small file
static void FS_fread_valid_flow_small_file()
{
    char write_buf[small_buf_size] = "good_day";
    char read_buf[small_buf_size] = {};

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}


//fread valid flow medium file
static void FS_fread_valid_flow_medium_file()
{
    char write_buf[medium_buf_size] = {1};
    char read_buf[medium_buf_size] = {};

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fread valid flow large file
static void FS_fread_valid_flow_large_file()
{
    char write_buf[large_buf_size] = {1};
    char read_buf[large_buf_size] = {};

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fread valid flow small file read more than write
static void FS_fread_valid_flow_small_file_read_more_than_write()
{
    char write_buf[small_buf_size] = "good_day";
    char read_buf[small_buf_size + 10] = {};

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

/*----------------fgetc()------------------*/

//fgetc to null ptr
static void FS_fgetc_null()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fgetc(NULL);
    TEST_ASSERT_EQUAL(1, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fgetc to an empty file
static void FS_fgetc_empty_file()
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

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fgetc(fd[0]);
    TEST_ASSERT_EQUAL(EOF, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fgetc valid flow
static void fgetc_valid_flow()
{
    char write_buf[small_buf_size] = "good_day";
    char read_buf[small_buf_size] = {};
    int ch = 0, i = 0;

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    for (i = 0; (i < (sizeof(read_buf)-1) && ((ch = fgetc(fd[0])) != EOF) && (ch != '\n')); i++) {
        read_buf[i] = ch;
    }
 
    read_buf[i] = '\0';

    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fgetc to fopen mode w
static void FS_fgetc_with_fopen_w_mode()
{
    char buffer[small_buf_size] = {};

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

    res = fgetc(fd[0]);
    TEST_ASSERT_EQUAL(EOF, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

/*----------------fgets()------------------*/

//fgets to null ptr
static void FS_fgets_null()
{
    char buffer[small_buf_size] = {};

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    TEST_ASSERT_NULL(fgets(buffer, sizeof(buffer), NULL));

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fgets to an empty file
static void FS_fgets_empty_file()
{
    char buffer[small_buf_size] = {};

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

    TEST_ASSERT_NULL(fgets(buffer, sizeof(buffer), fd[0]));

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fgets with buffer null with zero len of buffer string
static void FS_fgets_null_buffer_zero_len()
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

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    TEST_ASSERT_NULL(fgets(NULL, 0, fd[0]));

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fgets with buffer null
static void FS_fgets_null_buffer()
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

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    TEST_ASSERT_NULL(fgets(NULL, small_buf_size, fd[0]));

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fgets valid flow
static void FS_fgets_valid_flow()
{
    char write_buf[small_buf_size] = "good_day";
    char read_buf[small_buf_size] = {};

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    TEST_ASSERT_NOT_NULL(fgets(read_buf, sizeof(read_buf), fd[0]));
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fgets up to new line character
static void FS_fgets_new_line()
{
    char write_buf[small_buf_size] = "good_day";
    char read_buf[small_buf_size] = {};

    write_buf[4] = '\n';

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    TEST_ASSERT_NOT_NULL(fgets(read_buf, sizeof(read_buf), fd[0]));
    TEST_ASSERT_EQUAL_STRING("good\n", read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fgets with one character to read
static void FS_fgets_len_is_one()
{
    char write_buf[small_buf_size] = "good_day";
    char read_buf[small_buf_size] = {};

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = !(fgets(read_buf, small_buf_size, fd[0]) != NULL);

    TEST_ASSERT_EQUAL(0, res);
    TEST_ASSERT_EQUAL_STRING("", read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fgets to fopen mode w
static void FS_fgets_with_fopen_w_mode()
{
    char buffer[small_buf_size] = {};

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

    TEST_ASSERT_NULL(fgets(buffer, sizeof(buffer), fd[0]));

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

/*----------------fflush()------------------*/

//fflush with null
static void FS_fflush_null_stream()
{
    char buffer[small_buf_size] = {};

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fflush(NULL);
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}


//fflush valid flow
static void FS_fflush_valid_flow()
{
    char buffer[small_buf_size] = "good_day";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

    res = fflush(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}


//fflush twice
static void FS_fflush_twice()
{
    char buffer[small_buf_size] = "good_day";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

    res = fflush(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fflush(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

/*----------------fputc()------------------*/

//fputc with null
static void FS_fputc_null_stream()
{
    char buffer[small_buf_size] = {};
    int ch = 0;

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fputc(ch, NULL);
    TEST_ASSERT_EQUAL(1, res);

    deinit();
}

//fputc valid flow
static void FS_fputc_valid_flow()
{
    char buffer[small_buf_size] = "good_day";
    int write_ch = 10, read_ch = 0;

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fputc(write_ch, fd[0]);
    TEST_ASSERT_EQUAL(write_ch, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    read_ch = fgetc(fd[0]);
    TEST_ASSERT_EQUAL(write_ch, read_ch);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fputc char is not valid
static void FS_fputc_char_not_valid()
{
    char buffer[small_buf_size] = "good_day";
    int write_ch = 10000;

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fputc(write_ch, fd[0]);
    TEST_ASSERT_EQUAL(1000, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fputc with file open for read mode
static void FS_fputc_in_read_mode()
{
    char buffer[small_buf_size] = "good_day";
    int write_ch = 10;

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

    res = fputc(write_ch, fd[0]);
    TEST_ASSERT_EQUAL(EOF, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

/*----------------fputs()------------------*/

//fputs with null
static void FS_fputs_null_stream()
{
    char write_buf[small_buf_size] = "123456789";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fputs(write_buf, NULL);
    TEST_ASSERT_EQUAL(1, res);

    deinit();
}

//fputs valid flow
static void FS_fputs_valid_flow()
{
    char write_buf[small_buf_size] = "123456789";
    char read_buf[small_buf_size] = {};

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fputs(write_buf, fd[0]);
    TEST_ASSERT_NOT_EQUAL(EOF, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf) - 1, read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fputs with file open for read mode
static void FS_fputs_in_read_mode()
{
    char buffer[small_buf_size] = "good_day";

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

    res = fputs(buffer, fd[0]);
    TEST_ASSERT_EQUAL(EOF, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

/*----------------fseek()------------------*/

//fseek with null
static void FS_fseek_null_stream()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(NULL, 0, SEEK_SET);
    TEST_ASSERT_NOT_EQUAL(0, res);

    deinit();
}

//fseek empty file, SEEK_SET, offset 0
static void FS_fseek_empty_file_seek_set()
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

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 0, SEEK_SET);
    TEST_ASSERT_EQUAL(0, res);

    int pos = ftell(fd[0]);
    TEST_ASSERT_EQUAL(0, pos);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fseek non empty file, SEEK_SET, offset 0
static void FS_fseek_non_empty_file_seek_set()
{
    char write_buf[small_buf_size] = "123456789";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 0, SEEK_SET);
    TEST_ASSERT_EQUAL(0, res);

    int pos = ftell(fd[0]);
    TEST_ASSERT_EQUAL(0, pos);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fseek empty file, SEEK_SET, offset 1 - beyond end of file
static void FS_fseek_beyond_empty_file_seek_set()
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

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 10, SEEK_SET);
    printf("res = %d\n", res);
    TEST_ASSERT_NOT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fseek non empty file, SEEK_SET, offset data_size + 1 - beyond end of file
static void FS_fseek_beyond_non_empty_file_seek_set()
{
    char write_buf[small_buf_size] = "123456789";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], sizeof(write_buf) + 1, SEEK_SET);
    TEST_ASSERT_NOT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fseek empty file, SEEK_SET, offset -1 - before file start
static void FS_fseek_before_empty_file_seek_set()
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

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], -1, SEEK_SET);
    TEST_ASSERT_NOT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fseek empty file, SEEK_CUR, offset 0
static void FS_fseek_empty_file_seek_cur()
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

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 0, SEEK_CUR);
    TEST_ASSERT_EQUAL(0, res);

    int pos = ftell(fd[0]);
    TEST_ASSERT_EQUAL(0, pos);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fseek non empty file, SEEK_CUR, offset 0
static void FS_fseek_non_empty_file_seek_cur()
{
    char write_buf[small_buf_size] = "123456789";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 0, SEEK_CUR);
    TEST_ASSERT_EQUAL(0, res);

    int pos = ftell(fd[0]);
    TEST_ASSERT_EQUAL(0, pos);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fseek empty file, SEEK_CUR, offset 1 - beyond end of file
static void FS_fseek_beyond_empty_file_seek_cur()
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

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 1, SEEK_CUR);
    TEST_ASSERT_NOT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fseek non empty file, SEEK_CUR, offset data_size + 1 - beyond end of file
static void FS_fseek_beyond_non_empty_file_seek_cur()
{
    char write_buf[small_buf_size] = "123456789";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], sizeof(write_buf) + 1, SEEK_CUR);
    TEST_ASSERT_NOT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fseek empty file, SEEK_CUR, offset -1 - before file start
static void FS_fseek_before_empty_file_seek_cur()
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

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], -1, SEEK_CUR);
    TEST_ASSERT_NOT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fseek empty file, SEEK_END, offset 0
static void FS_fseek_empty_file_seek_end()
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

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 0, SEEK_END);
    TEST_ASSERT_EQUAL(0, res);

    int pos = ftell(fd[0]);
    TEST_ASSERT_EQUAL(0, pos);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fseek non empty file, SEEK_END, offset 0
static void FS_fseek_non_empty_file_seek_end()
{
    char write_buf[small_buf_size] = "123456789";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 0, SEEK_END);
    TEST_ASSERT_EQUAL(0, res);

    int pos = ftell(fd[0]);
    TEST_ASSERT_EQUAL(write_sz, pos);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fseek empty file, SEEK_END, offset 1 - beyond end of file
static void FS_fseek_beyond_empty_file_seek_end()
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

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 1, SEEK_END);
    TEST_ASSERT_NOT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fseek non empty file, SEEK_END, offset data_size + 1 - beyond end of file
static void FS_fseek_beyond_non_empty_file_seek_end()
{
    char write_buf[small_buf_size] = "123456789";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], sizeof(write_buf) + 1, SEEK_END);
    TEST_ASSERT_NOT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fseek empty file, SEEK_END, offset -1 - before file start
static void FS_fseek_before_empty_file_seek_end()
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

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], -1, SEEK_END);
    TEST_ASSERT_NOT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fseek non empty file, SEEK_END, offset negative
static void FS_fseek_negative_non_empty_file_seek_end()
{
    char write_buf[small_buf_size] = "123456789";

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], -(sizeof(write_buf)), SEEK_END);
    TEST_ASSERT_EQUAL(0, res);

    int pos = ftell(fd[0]);
    TEST_ASSERT_EQUAL(0, pos);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

/*----------------ftell()------------------*/

//ftell with null
static void FS_ftell_null_stream()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = ftell(NULL);
    TEST_ASSERT_EQUAL(-1, res);

    deinit();
}

/*----------------feof()------------------*/

//feof with null
static void FS_feof_null_stream()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = ftell(NULL);
    TEST_ASSERT_EQUAL(-1, res);

    deinit();
}

/*----------------fsetpos()------------------*/

//fsetpos with null
static void FS_fsetpos_null_stream()
{
    fpos_t pos;

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fsetpos (NULL, &pos);
    TEST_ASSERT_NOT_EQUAL(0, res);

    deinit();
}

/*----------------fgetpos()------------------*/

//fgetpos with null
static void FS_fgetpos_null_stream()
{
    fpos_t pos;

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fgetpos (NULL, &pos);
    TEST_ASSERT_NOT_EQUAL(0, res);

    deinit();
}

//fgetpos rewrite file and check data
static void FS_fgetpos_rewrite_check_data()
{
    char write_buf[small_buf_size] = "123456789";
    char rewrite_buf[small_buf_size] = "987654321";
    char read_buf[small_buf_size] = {};
    fpos_t pos;

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fgetpos(fd[0], &pos);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fsetpos(fd[0], &pos);
    TEST_ASSERT_EQUAL(0, res);

    write_sz = fwrite(rewrite_buf, sizeof(char), sizeof(rewrite_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(rewrite_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(rewrite_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(rewrite_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

/*----------------fscanf()------------------*/

//fscanf with null
static void FS_fscanf_null_stream()
{
    char read_buf[small_buf_size] = {};

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fscanf(NULL, "%s", read_buf);
    TEST_ASSERT_NOT_EQUAL(EOF, res);

    deinit();
}

//fscanf valid flow
static void FS_fscanf_valid_flow()
{
    char write_buf[small_buf_size] = "123456789";
    char read_buf[small_buf_size] = {};
    int num = 0;
    fpos_t pos;

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fprintf(fd[0], "%d %s", 123, write_buf);
    TEST_ASSERT_EQUAL(3 + sizeof(write_buf), res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fscanf(fd[0], "%d", &num);
    TEST_ASSERT_EQUAL(1, res);
    TEST_ASSERT_EQUAL_INT(123, num);

    res = fscanf(fd[0], "%s", read_buf);
    TEST_ASSERT_EQUAL(1, res);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fscanf empty file
static void FS_fscanf_empty_file()
{
    char read_buf[small_buf_size] = {};
    int num = 0;

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fscanf(fd[0], "%d", &num);
    TEST_ASSERT_EQUAL(EOF, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//fscanf more fields than exist
static void FS_fscanf_more_fields_than_exist()
{
    char write_buf[small_buf_size] = "123456789";
    char read_buf[small_buf_size] = {};
    int num = 0;
    fpos_t pos;

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fprintf(fd[0], "%d %s", 123, write_buf);
    TEST_ASSERT_EQUAL(3 + sizeof(write_buf), res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fscanf(fd[0], "%d", &num);
    TEST_ASSERT_EQUAL(1, res);
    TEST_ASSERT_EQUAL_INT(123, num);

    res = fscanf(fd[0], "%s", read_buf);
    TEST_ASSERT_EQUAL(1, res);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fscanf(fd[0], "%d", &num);
    TEST_ASSERT_EQUAL(EOF, res);
    
    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

/*----------------fprintf()------------------*/

//fprintf with null
static void FS_fprintf_null_stream()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fprintf(NULL, "%d %s", 123, "PI");
    TEST_ASSERT_EQUAL(EOF, res);

    deinit();
}

//fprintf in mode r
static void FS_fprintf_read_mode()
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

    res = !((fd[0] = fopen("/lfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fprintf(fd[0], "%d", 123);
    TEST_ASSERT_NOT_EQUAL(1, res);
    
    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

/*----------------freopen()------------------*/

//freopen with null
static void FS_freopen_null_stream()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = freopen(NULL, "wb", NULL)) != NULL);
    TEST_ASSERT_EQUAL(EOF, res);

    deinit();
}

//freopen point to the same file with two file handler
static void FS_freopen_point_to_same_file()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[1] = freopen("/lfs/" "new_file_name", "wb", fd[0])) != NULL);
    TEST_ASSERT_EQUAL(0, res);
    
    TEST_ASSERT_EQUAL(fd[0], fd[1]);
    
    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//freopen change open file mode
static void FS_freopen_change_file_mode()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[1] = freopen("", "rb", fd[0])) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//freopen invalid mode
static void FS_freopen_invalid_mode()
{
    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[1] = freopen("", "", fd[0])) != NULL);
    TEST_ASSERT_EQUAL(1, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

//freopen valid flow
static void FS_freopen_valid_flow()
{
    char write_buf[small_buf_size] = "123456789";
    char read_buf[small_buf_size] = {};

    init();

    LittleFileSystem fs("lfs");

    int res = LittleFileSystem::format(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.mount(&bd);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/lfs/" "hello", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = !((fd[1] = freopen("", "rb", fd[0])) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fs.unmount();
    TEST_ASSERT_EQUAL(0, res);

    deinit();
}

/*----------------setup------------------*/

Case cases[] = {
    Case("FS_fopen_path_not_valid", FS_fopen_path_not_valid),
    Case("FS_fopen_empty_path_r_mode", FS_fopen_empty_path_r_mode),
    Case("FS_fopen_empty_path_w_mode", FS_fopen_empty_path_w_mode),
    Case("FS_fopen_invalid_mode", FS_fopen_invalid_mode),
    Case("FS_fopen_supported_wb_mode", FS_fopen_supported_wb_mode),
    Case("FS_fopen_supported_a_mode", FS_fopen_supported_a_mode),
    Case("FS_fopen_supported_r_mode", FS_fopen_supported_r_mode),
    Case("FS_fopen_supported_a_update_mode", FS_fopen_supported_a_update_mode),
    Case("FS_fopen_supported_r_update_mode", FS_fopen_supported_r_update_mode),
    Case("FS_fopen_supported_w_update_mode", FS_fopen_supported_w_update_mode),
    Case("FS_fopen_read_update_create", FS_fopen_read_update_create),
    Case("FS_fopen_write_update_create", FS_fopen_write_update_create),

    Case("FS_fclose_valid_flow", FS_fclose_valid_flow),
    Case("FS_fclose_null", FS_fclose_closed_file),  //should be EOF

    Case("FS_fwrite_nmemb_zero", FS_fwrite_nmemb_zero),
    Case("FS_fwrite_valid_flow", FS_fwrite_valid_flow),
    Case("FS_fwrite_with_fopen_r_mode", FS_fwrite_with_fopen_r_mode),
    //Case("FS_fwrite_stream_null", FS_fwrite_stream_null),   //mbed Hardfault
    //Case("FS_fwrite_buff_null", FS_fwrite_buff_null),     //FAIL: Expected 0 Was 10
    //Case("FS_fwrite_size_zero", FS_fwrite_size_zero), //FAIL: Expected 0 Was 10

    Case("FS_fread_size_zero", FS_fread_size_zero),
    Case("FS_fread_nmemb_zero", FS_fread_nmemb_zero),
    Case("FS_fread_with_fopen_w_mode", FS_fread_with_fopen_w_mode),
    //Case("FS_fread_with_ptr_null", FS_fread_with_ptr_null), //mbed Hardfault
    Case("FS_fread_to_fwrite_file", FS_fread_to_fwrite_file),
    Case("FS_fread_empty_file", FS_fread_empty_file),
    Case("FS_fread_valid_flow_small_file", FS_fread_valid_flow_small_file),
    Case("FS_fread_valid_flow_medium_file", FS_fread_valid_flow_medium_file),
    Case("FS_fread_valid_flow_large_file", FS_fread_valid_flow_large_file),
    Case("FS_fread_valid_flow_small_file_read_more_than_write", FS_fread_valid_flow_small_file_read_more_than_write),

    //Case("FS_fgetc_null", FS_fgetc_null),  //mbed Hardfault
    Case("FS_fgetc_empty_file", FS_fgetc_empty_file),
    Case("fgetc_valid_flow", fgetc_valid_flow),
    Case("FS_fgetc_with_fopen_w_mode", FS_fgetc_with_fopen_w_mode),

    Case("FS_fgets_empty_file", FS_fgets_empty_file),
    Case("FS_fgets_null_buffer_zero_len", FS_fgets_null_buffer_zero_len),
    Case("FS_fgets_null_buffer", FS_fgets_null_buffer),
    Case("FS_fgets_valid_flow", FS_fgets_valid_flow),
    Case("FS_fgets_new_line", FS_fgets_new_line),
    //Case("FS_fgets_len_is_one", FS_fgets_len_is_one), //FAIL: Expected '' Was 'good_day'
    Case("FS_fgets_with_fopen_w_mode", FS_fgets_with_fopen_w_mode),

    Case("FS_fflush_null_stream", FS_fflush_null_stream),
    Case("FS_fflush_valid_flow", FS_fflush_valid_flow),
    Case("FS_fflush_twice", FS_fflush_twice),

    //Case("FS_fputc_null_stream", FS_fputc_null_stream), //mbed Hardfault
    Case("FS_fputc_valid_flow", FS_fputc_valid_flow),
    //Case("FS_fputc_char_not_valid", FS_fputc_char_not_valid), //depracated
    Case("FS_fputc_in_read_mode", FS_fputc_in_read_mode),

    //Case("FS_fputs_null_stream", FS_fputs_null_stream),  //mbed Hardfault
    Case("FS_fputs_valid_flow", FS_fputs_valid_flow),
    Case("FS_fputs_in_read_mode", FS_fputs_in_read_mode),

    Case("FS_fseek_null_stream", FS_fseek_null_stream),
    Case("FS_fseek_empty_file_seek_set", FS_fseek_empty_file_seek_set),
    Case("FS_fseek_non_empty_file_seek_set", FS_fseek_non_empty_file_seek_set),
    //Case("FS_fseek_empty_file_seek_set", FS_fseek_beyond_empty_file_seek_set), //fseek doesnt fail: Expected Not-Equal
    //Case("FS_fseek_beyond_non_empty_file_seek_set", FS_fseek_beyond_non_empty_file_seek_set), //fseek doesnt fail: Expected Not-Equal
    Case("FS_fseek_before_empty_file_seek_set", FS_fseek_before_empty_file_seek_set),
    Case("FS_fseek_empty_file_seek_cur", FS_fseek_empty_file_seek_cur),
    Case("FS_fseek_non_empty_file_seek_cur", FS_fseek_non_empty_file_seek_cur),
    //Case("FS_fseek_empty_file_seek_cur", FS_fseek_beyond_empty_file_seek_cur),//fseek doesnt fail: Expected Not-Equal
    //Case("FS_fseek_beyond_non_empty_file_seek_cur", FS_fseek_beyond_non_empty_file_seek_cur),//fseek doesnt fail: Expected Not-Equal
    Case("FS_fseek_before_empty_file_seek_cur", FS_fseek_before_empty_file_seek_cur),
    Case("FS_fseek_empty_file_seek_end", FS_fseek_empty_file_seek_end),
    Case("FS_fseek_non_empty_file_seek_end", FS_fseek_non_empty_file_seek_end),
    //Case("FS_fseek_empty_file_seek_end", FS_fseek_beyond_empty_file_seek_end),////fseek doesnt fail: Expected Not-Equal
    //Case("FS_fseek_beyond_non_empty_file_seek_end", FS_fseek_beyond_non_empty_file_seek_end),////fseek doesnt fail: Expected Not-Equal
    Case("FS_fseek_before_empty_file_seek_end", FS_fseek_before_empty_file_seek_end),
    Case("FS_fseek_negative_non_empty_file_seek_end", FS_fseek_negative_non_empty_file_seek_end),

    Case("FS_ftell_null_stream", FS_ftell_null_stream),

    Case("FS_feof_null_stream", FS_feof_null_stream),

    Case("FS_fsetpos_null_stream", FS_fsetpos_null_stream),

    Case("FS_fgetpos_null_stream", FS_fgetpos_null_stream),
    Case("FS_fgetpos_rewrite_check_data", FS_fgetpos_rewrite_check_data),

    //Case("FS_fscanf_null_stream", FS_fscanf_null_stream), //mbed Hardfault
    Case("FS_fscanf_valid_flow", FS_fscanf_valid_flow),
    Case("FS_fscanf_empty_file", FS_fscanf_empty_file),
    Case("FS_fscanf_more_fields_than_exist", FS_fscanf_more_fields_than_exist),

    //Case("FS_fprintf_null_stream", FS_fprintf_null_stream),
    Case("FS_fprintf_read_mode", FS_fprintf_read_mode),

    //Case("FS_freopen_null_stream", FS_freopen_null_stream),
    Case("FS_freopen_point_to_same_file", FS_freopen_point_to_same_file),
    //Case("FS_freopen_change_file_mode", FS_freopen_change_file_mode),//TIMEOUT
    Case("FS_freopen_invalid_mode", FS_freopen_invalid_mode),
    //Case("FS_freopen_valid_flow", FS_freopen_valid_flow),//UnicodeDecodeError: 'utf8' codec can't decode byte 0xc6 in position 2: invalid continuation byte

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
