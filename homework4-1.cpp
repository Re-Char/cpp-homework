#include "homework4-1.h"
// #include "CowString.h"

int StringData::next_id = 0;

CowString::CowString(const char *str) {
    // TODO
    _ref_count = new int(1);
    _data = new StringData(str);
}

CowString::~CowString() {
    // TODO
    (*_ref_count)--;
    if (*_ref_count == 0)
        delete _data;
}

CowString::CowString(const CowString &other) : _data(other._data), _ref_count(other._ref_count) {
    // TODO
    (*_ref_count)++;
}

char CowString::read(int index) const {
    // TODO
    return _data->buffer[index];
}

void CowString::write(int index, char c) {
    if (*_ref_count > 1)
    {
        (*_ref_count)--;
        _data = new StringData(*_data);
        _ref_count = new int(1);
    }
    _data->buffer[index] = c;
}