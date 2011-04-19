/*
 * $Id$
 *
 */

#ifndef SQUID_IPC_MEM_SEGMENT_H
#define SQUID_IPC_MEM_SEGMENT_H

#include "SquidString.h"

namespace Ipc {

namespace Mem {

/// POSIX shared memory segment
class Segment {
public:
    /// Create a shared memory segment.
    Segment(const char *const id);
    ~Segment();

    /// Create a new shared memory segment. Fails if a segment with
    /// the same name already exists.
    void create(const off_t aSize);
    void open(); ///< Open an existing shared memory segment.

    const String &name() { return theName; } ///< shared memory segment name
    off_t size() { return theSize; } ///< shared memory segment size
    void *mem() { return reserve(0); } ///< pointer to the next chunk
    void *reserve(size_t chunkSize); ///< reserve and return the next chunk

    static void Unlink(const char *const id); ///< unlink the segment

private:
    void attach();
    void detach();
    off_t statSize(const char *context) const;

    static String GenerateName(const char *id);

    const String theName; ///< shared memory segment file name
    int theFD; ///< shared memory segment file descriptor
    void *theMem; ///< pointer to mmapped shared memory segment
    off_t theSize; ///< shared memory segment size
    off_t theReserved; ///< the total number of reserve()d bytes
};

} // namespace Mem

} // namespace Ipc

#endif /* SQUID_IPC_MEM_SEGMENT_H */
