/* Flags for IO initialization */
#define IOI_LOCALONLY   0x1     /* Operate only on local quota */
#define IOI_READONLY    0x2     /* Only readonly access */
#define IOI_OPENFILE    0x4     /* Open file even if kernel has quotas 
turned on */

#define FL_QUIET 1
#define FL_VERBOSE 2
#define FL_USER 4
#define FL_GROUP 8
#define FL_SMARTSIZE 16
#define FL_LOCALONLY 32
#define FL_QUIETREFUSE 64


#define MNTTYPE_EXT2           "ext2"  /* 2nd Extended file system */
#define MNTTYPE_EXT3           "ext3"  /* ext2 + journaling */
#define MNTTYPE_MINIX          "minix" /* MINIX file system */
#define MNTTYPE_UFS            "ufs"   /* UNIX file system */
#define MNTTYPE_UDF            "udf"   /* OSTA UDF file system */
#define MNTTYPE_REISER         "reiserfs"      /* Reiser file system */
#define MNTTYPE_XFS            "xfs"   /* SGI XFS file system */

/* mount options */
#define MNTOPT_NOQUOTA          "noquota"       /* don't enforce quota */
#define MNTOPT_QUOTA            "quota" /* enforce user quota */
#define MNTOPT_USRQUOTA         "usrquota"      /* enforce user quota */
#define MNTOPT_GRPQUOTA         "grpquota"      /* enforce group quota */
#define MNTOPT_RSQUASH          "rsquash"       /* root as ordinary user 
*/
#define MNTOPT_BIND             "bind"          /* binded mount */
#define MNTOPT_LOOP             "loop"          /* loopback mount */

/*
 * Disk quota - quotactl(2) commands for XFS Quota Manager (XQM).
 */
#define Q_XFS_QUOTAON   XQM_CMD(0x1)       /* enable quota 
accounting/enforcement */
#define Q_XFS_QUOTAOFF  XQM_CMD(0x2)       /* disable quota 
accounting/enforcement */
#define Q_XFS_GETQUOTA  XQM_CMD(0x3)       /* get disk limits & usage */
#define Q_XFS_SETQLIM   XQM_CMD(0x4)       /* set disk limits only */
#define Q_XFS_GETQSTAT  XQM_CMD(0x5)       /* returns fs_quota_stat_t 
struct */
#define Q_XFS_QUOTARM   XQM_CMD(0x6)       /* free quota files' space */

#define XFS_QUOTA_UDQ_ACCT      (1<<0)  /* user quota accounting */
#define XFS_QUOTA_UDQ_ENFD      (1<<1)  /* user quota limits enforcement 
*/
#define XFS_QUOTA_GDQ_ACCT      (1<<2)  /* group quota accounting */
#define XFS_QUOTA_GDQ_ENFD      (1<<3)  /* group quota limits enforcement 
*/

#define XFS_USER_QUOTA          (1<<0)  /* user quota type */
#define XFS_PROJ_QUOTA          (1<<1)  /* (IRIX) project quota type */
#define XFS_GROUP_QUOTA         (1<<2)  /* group quota type */

typedef struct fs_qfilestat {
        __u64 qfs_ino;          /* inode number */
        __u64 qfs_nblks;        /* number of BBs 512-byte-blks */
        __u32 qfs_nextents;     /* number of extents */
} fs_qfilestat_t;

typedef struct fs_quota_stat {
        __s8 qs_version;        /* version number for future changes */
        __u16 qs_flags;         /* XFS_QUOTA_{U,P,G}DQ_{ACCT,ENFD} */
        __s8 qs_pad;            /* unused */
        fs_qfilestat_t qs_uquota;       /* user quota storage information */
        fs_qfilestat_t qs_gquota;       /* group quota storage information */
        __u32 qs_incoredqs;     /* number of dquots incore */
        __s32 qs_btimelimit;    /* limit for blks timer */
        __s32 qs_itimelimit;    /* limit for inodes timer */
        __s32 qs_rtbtimelimit;  /* limit for rt blks timer */
        __u16 qs_bwarnlimit;    /* limit for num warnings */
        __u16 qs_iwarnlimit;    /* limit for num warnings */
} fs_quota_stat_t;

#define xfs_mem_dqinfo      fs_quota_stat

#define XQM_CMD(cmd)    ( ('X'<<8)+(cmd) )

