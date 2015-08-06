/*
 * Copyright (C) 2012 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _LIB_SECION_H_
#define _LIB_SECION_H_

#include <unistd.h> /* size_t */


/* ion_client
 * An ION client is an object or an entity that needs to use the service of
 * ION and has unique address space. ion_client is an identifier of an ION
 * client and it represents the ION client.
 * All operations on ION needs a valid ion_client value and it can be obtained
 * by ion_client_create().
 */
typedef int ion_client;

/* ion_buffer
 * An identifier of a buffer allocated from ION. You must obtain to access
 * a buffer allocated from ION. If you have an effective ion_buffer, you have
 * three options to work with it.
 * - To access  the buffer, you can request an address (user virtual address)
 *   of the buffer with ion_map().
 * - To pass the buffer to the kernel, you can pass the ion_buffer to the
 *   kernel driver directly, if the kernel driver can work with ION.
 * - To pass the buffer to other processes, you can pass the ion_buffer to
 *   other processes through RPC machanism such as socket communication or
 *   Android Binder because ion_buffer is actually an open file descripotor
 *   of the current process.
 */
typedef int ion_buffer;

typedef unsigned long ion_phys_addr_t;


#define ION_HEAP_SYSTEM_MASK		(1 << 0)
#define ION_HEAP_SYSTEM_CONTIG_MASK	(1 << 1)
#define ION_HEAP_EXYNOS_CONTIG_MASK	(1 << 4)
#define ION_HEAP_EXYNOS_MASK		(1 << 5)
#define ION_HEAP_EXYNOS_USER_MASK	(1 << 6)
#define ION_EXYNOS_NONCACHE_MASK	(1 << (32 - 2)) /* it was BITS_PER_LONG */
#define ION_EXYNOS_WRITE_MASK		(1 << (32 - 1)) /* it was BITS_PER_LONG */

/* ION_MSYNC_FLAGS
 * values of @flags parameter to ion_msync()
 *
 * IMSYNC_DEV_TO_READ: Device only reads the buffer
 * IMSYNC_DEV_TO_WRITE: Device may writes to the buffer
 * IMSYNC_DEV_TO_RW: Device reads and writes to the buffer
 *
 * IMSYNC_SYNC_FOR_DEV: ion_msync() for device to access the buffer
 * IMSYNC_SYNC_FOR_CPU: ion_msync() for CPU to access the buffer after device
 *                      has accessed it.
 *
 * The values must be ORed with one of IMSYNC_DEV_* and one of IMSYNC_SYNC_*.
 * Otherwise, ion_msync() will not effect.
 */
enum ION_MSYNC_FLAGS {
    IMSYNC_DEV_TO_READ = 0,
    IMSYNC_DEV_TO_WRITE = 1,
    IMSYNC_DEV_TO_RW = 2,
    IMSYNC_SYNC_FOR_DEV = 0x10000,
    IMSYNC_SYNC_FOR_CPU = 0x20000,
};

struct secion_param {
    ion_client client;
    ion_buffer buffer;
    size_t size;
    void *memory;
    ion_phys_addr_t physaddr;
};

#ifdef __cplusplus
extern "C" {
#endif

ion_client ion_client_create(void);
void ion_client_destroy(ion_client client);
ion_buffer ion_alloc(ion_client client, size_t len, size_t align, unsigned int flags);
void ion_free(ion_buffer buffer);
void *ion_map(ion_buffer buffer, size_t len, off_t offset);
int ion_unmap(void *addr, size_t len);

/* ion_msync() - Makes sure that data in the buffer are visible to H/W peri.
 * @client: A valid ion_client value returned by ion_client_create().
 * @buffer: The buffer to perform ion_msync().
 * @flags: Direction of access of H/W peri and CPU. See the description of
 *         ION_MSYNC_FLAGS.
 * @size: Size to ion_msync() in bytes.
 * @offset: Where ion_msync() start in @buffer, size in bytes.
 * @RETURN: 0 if successful. -error, otherwise.
 *
 * Note that @offset + @size must not exceed the size of @buffer.
 */
int ion_msync(ion_client client, ion_buffer buffer, long flags, size_t size, off_t offset);




ion_phys_addr_t ion_getphys(ion_client client, ion_buffer buffer);
int createIONMem(struct secion_param *param, size_t size, unsigned int flags);
int destroyIONMem(struct secion_param *param);

#ifdef __cplusplus
}
#endif
#endif /* _LIB_SECION_H_ */
