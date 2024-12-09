#ifndef _TYPES_H_
#define _TYPES_H_

typedef int boolean;

/******************************************************************************
 * SECTION: Macro
 *******************************************************************************/
#define TRUE 1
#define FALSE 0
#define UINT32_BITS 32
#define UINT8_BITS 8

#define NFS_MAGIC_NUM 0x52415453
#define NFS_SUPER_OFS 0
#define NFS_ROOT_INO 0

#define NFS_ERROR_NONE 0
#define NFS_ERROR_ACCESS EACCES
#define NFS_ERROR_SEEK ESPIPE
#define NFS_ERROR_ISDIR EISDIR
#define NFS_ERROR_NOSPACE ENOSPC
#define NFS_ERROR_EXISTS EEXIST
#define NFS_ERROR_NOTFOUND ENOENT
#define NFS_ERROR_UNSUPPORTED ENXIO
#define NFS_ERROR_IO EIO       /* Error Input/Output */
#define NFS_ERROR_INVAL EINVAL /* Invalid Args */

#define NFS_MAX_FILE_NAME 128
#define NFS_INODE_PER_FILE 1
#define NFS_MAX_SIZE_PER_FILE 16
#define NFS_DEFAULT_PERM 0777

#define NFS_IOC_MAGIC 'S'
#define NFS_IOC_SEEK _IO(NFS_IOC_MAGIC, 0)

#define NFS_FLAG_BUF_DIRTY 0x1
#define NFS_FLAG_BUF_OCCUPY 0x2
/******************************************************************************
 * SECTION: Macro Function
 *******************************************************************************/
#define NFS_LOGIC_SZ() (newfs_super.sz_logic)
#define NFS_IO_SZ() (newfs_super.sz_io)
#define NFS_DISK_SZ() (newfs_super.sz_disk)
#define NFS_DRIVER() (newfs_super.driver_fd)

#define NFS_ROUND_DOWN(value, round) ((value) % (round) == 0 ? (value) : ((value) / (round)) * (round))
#define NFS_ROUND_UP(value, round) ((value) % (round) == 0 ? (value) : ((value) / (round) + 1) * (round))

#define NFS_BLKS_SZ(blks) ((blks) * NFS_LOGIC_SZ())
#define NFS_ASSIGN_FNAME(pnewfs_dentry, _fname) memcpy((pnewfs_dentry)->name, (_fname), strlen((_fname)))
// data和inode的布局不一样，所以offset计算方式也不同
// 多个ino可以在同一个块内，一个dno代表一个块
#define NFS_INO_OFS(ino) (NFS_BLKS_SZ(newfs_super.ino_offset) + (ino) * sizeof(struct newfs_inode_d))
#define NFS_DATA_OFS(dno) (NFS_BLKS_SZ(newfs_super.data_offset + (dno)))

#define NFS_IS_DIR(pinode) ((pinode)->dentry->ftype == NFS_DIR)
#define NFS_IS_REG(pinode) ((pinode)->dentry->ftype == NFS_REG_FILE)
struct newfs_dentry;
struct newfs_inode;
struct newfs_super;

typedef enum newfs_file_type
{
    NFS_REG_FILE,
    NFS_DIR,
} NFS_FILE_TYPE;

struct custom_options
{
    char *device;
    boolean show_help;
};

struct newfs_super
{
    uint32_t magic;

    /* TODO: Define yourself */

    boolean is_mounted;
    int driver_fd;
    int sz_io;
    int sz_logic;
    int sz_disk;
    int sz_usage;

    int sb_offset;       // 0
    int sb_blks;         // 1
    int ino_map_offset;  // 1
    int ino_map_blks;    // 1
    int data_map_offset; // 2
    int data_map_blks;   // 1
    int ino_offset;      // 3
    int ino_blks;        // = (max_file * sizeof(inode_d)) / sz_logic_blk
    int data_offset;
    int data_blks;

    uint8_t *map_inode;
    uint8_t *map_data;

    struct newfs_dentry *root_dentry;
};

struct newfs_inode
{
    /* inode编号 */
    // ino >= 0 && ino < ino_blks
    int ino; /* 在inode位图中的下标 */
    /* 文件的属性 */
    int size;                    /* 文件已占用空间 */
    NFS_FILE_TYPE ftype;         // 文件类型（目录类型、普通文件类型）
    struct newfs_dentry *dentry; /* 指向该inode的dentry */

    int block_pointer[NFS_MAX_SIZE_PER_FILE]; // 数据块指针

    /* 目录 */
    int dir_cnt;
    struct newfs_dentry *dentrys; /* 所有目录项 */

    /* 文件 */
    uint8_t *data;
};

struct newfs_dentry
{
    char name[NFS_MAX_FILE_NAME];
    uint32_t ino;
    /* TODO: Define yourself */
    NFS_FILE_TYPE ftype;
    struct newfs_dentry *parent;  /* 父亲Inode的dentry */
    struct newfs_dentry *brother; /* 兄弟 */
    struct newfs_inode *inode;    /* 指向inode */
};

/******************************************************************************
 * SECTION: FS Specific Structure - Disk structure
 *******************************************************************************/
struct newfs_super_d
{
    uint32_t magic_num;
    int sz_usage;

    int sb_offset;       // 0
    int sb_blks;         // 1
    int ino_map_offset;  // 1
    int ino_map_blks;    // 1
    int data_map_offset; // 2
    int data_map_blks;   // 1
    int ino_offset;      // 3
    int ino_blks;        // = (max_file * sizeof(inode_d)) / sz_logic_blk
    int data_offset;
    int data_blks;
};

struct newfs_inode_d
{
    /* inode编号 */
    int ino; /* 在inode位图中的下标 */
    /* 文件的属性 */
    int size;            /* 文件已占用空间 */
    NFS_FILE_TYPE ftype; // 文件类型（目录类型、普通文件类型）

    int block_pointer[NFS_MAX_SIZE_PER_FILE]; // 数据块指针（可固定分配）
    int dir_cnt;
};

#define INODE_PER_BLK (NFS_LOGIC_SZ() / sizeof(struct newfs_inode_d))

struct newfs_dentry_d
{
    char fname[NFS_MAX_FILE_NAME];
    NFS_FILE_TYPE ftype;
    int ino; /* 指向的ino号 */
};

#define DENTRY_PER_BLK (NFS_LOGIC_SZ() / sizeof(struct newfs_dentry_d))

// ####################### Functions #######################
static inline struct newfs_dentry *new_dentry(char *fname, NFS_FILE_TYPE ftype)
{
    struct newfs_dentry *dentry = (struct newfs_dentry *)malloc(sizeof(struct newfs_dentry));
    memset(dentry, 0, sizeof(struct newfs_dentry));
    NFS_ASSIGN_FNAME(dentry, fname);
    dentry->ftype = ftype;
    dentry->ino = -1;
    dentry->inode = NULL;
    dentry->parent = NULL;
    dentry->brother = NULL;
}
#endif /* _TYPES_H_ */