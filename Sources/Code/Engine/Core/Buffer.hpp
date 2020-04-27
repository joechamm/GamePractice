#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <memory>
#include <string>

struct BufferImpl;

class Buffer {
public:

    Buffer();
    Buffer(const Buffer& buff);
    Buffer(Buffer&& buff);
    Buffer(const void* dataPtr, size_t dataSize);
    explicit Buffer(size_t buffSize);

    ~Buffer() = default;

    Buffer& operator=(Buffer&& buff);
    Buffer& operator=(const Buffer& buff);
    explicit operator bool() const;

    std::string getString() const;

    size_t getSize() const;
    void* getWriteData();
    const void* getReadData() const;
    void resize(size_t newSize);

private:

    std::shared_ptr<BufferImpl> buffImpl;
};

#endif /* __BUFFER_HPP__ */