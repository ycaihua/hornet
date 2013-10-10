#ifndef HORNET_JAVA_HH
#define HORNET_JAVA_HH

#include <valarray>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <mutex>

namespace hornet {

struct method;
struct field;
struct klass;

struct cp_info {
    virtual ~cp_info() {
    }
};

struct const_class_info : cp_info {
    uint16_t name_index;
};

struct const_utf8_info : cp_info {
    ~const_utf8_info() {
        delete[] bytes;
    }

    char *bytes;
};

class constant_pool {
public:
    explicit constant_pool(uint16_t size);
    ~constant_pool();

    void set(uint16_t idx, std::shared_ptr<cp_info> entry);

    const_utf8_info *get_utf8(uint16_t idx);

private:
    std::valarray<std::shared_ptr<cp_info>> _entries;
};

enum class attr_type {
    code,
    unknown
};

struct attr_info {
    attr_type type;

    attr_info(attr_type type) : type(type) {}
};

struct code_attr : attr_info {
    char*    code;
    uint32_t code_length;

    code_attr() : attr_info(attr_type::code) {}
};

struct unknown_attr : attr_info {
    unknown_attr() : attr_info(attr_type::unknown) {}
};

class class_file {
public:
    explicit class_file(void *data, size_t size);
    ~class_file();

    std::shared_ptr<klass> parse();
private:

    std::shared_ptr<cp_info> read_cp_info();
    std::shared_ptr<const_class_info> read_const_class();
    void read_const_fieldref();
    void read_const_methodref();
    void read_const_string();
    void read_const_name_and_type();
    std::shared_ptr<const_utf8_info> read_const_utf8();

    std::shared_ptr<field> read_field_info();
    std::shared_ptr<method> read_method_info(constant_pool &constant_pool);
    std::unique_ptr<attr_info> read_attr_info(constant_pool &constant_pool);
    std::unique_ptr<code_attr> read_code_attribute(constant_pool &constant_pool);

    uint8_t  read_u1();
    uint16_t read_u2();
    uint32_t read_u4();
    uint64_t read_u8();

    size_t _offset;
    size_t _size;
    char *_data;
};

}

#endif
