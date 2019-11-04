#define FUSE_USE_VERSION 26

#include <fuse_lowlevel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/wait.h>
#include "tosfs.h"





static int main_stat(fuse_ino_t ino, struct stat *stbuf)
{
	//Récupération du descripteur de fichier à mapper
	int fd = open("test_tosfs_files", O_RDWR);
	printf("Le descripteur de fichier fd= %d\n", fd);

	//Création d'un segment en mémoire avec le texte de test_tosfs_files inscrit à cet emplacement
	void *mmappedData = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	printf("Adresse de la mmap: %p\n\n",mmappedData);

	//Récupération des informations sur le système de fichier: SUPERBLOCK
	struct tosfs_superblock* SB;
	SB = (struct tosfs_superblock*) mmappedData;
	//Récupération des numéros d'inodes et d'autres informations (pointeur vers les fichiers) associés aux différents fichiers
	struct tosfs_inode *inode;
	inode = (struct tosfs_inode*) ((void*) SB + SB->block_size);

	stbuf->st_ino = ino;

	inode+=ino;
	stbuf->st_mode = inode->mode;
	stbuf->st_nlink=inode->nlink;
	stbuf->st_size=inode->size;
	stbuf->st_gid=inode->gid;
	stbuf->st_uid=inode->uid;
	/*switch (ino) {
	case 1:
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		break;

	case 2:
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen(hello_str);
		break;

	default:
		return -1;
	}*/
	return 0;
}


static void main_ll_getattr(fuse_req_t req, fuse_ino_t ino,
			     struct fuse_file_info *fi) {
	struct stat stbuf;

	(void) fi;

	memset(&stbuf, 0, sizeof(stbuf));
	if (main_stat(ino, &stbuf) == -1)
		fuse_reply_err(req, ENOENT);
	else
		fuse_reply_attr(req, &stbuf, 1.0);
}


/*static void main_ll_lookup(fuse_req_t req, fuse_ino_t parent, const char *name) {

}



static void main_ll_readdir(fuse_req_t req, fuse_ino_t ino, size_t size,
			     off_t off, struct fuse_file_info *fi) {

}

static void main_ll_open(fuse_req_t req, fuse_ino_t ino,
			  struct fuse_file_info *fi) {

}

static void main_ll_read(fuse_req_t req, fuse_ino_t ino, size_t size,
			  off_t off, struct fuse_file_info *fi) {

}*/

static struct fuse_lowlevel_ops main_ll_oper = {
	//.lookup		= main_ll_lookup,
	.getattr	= main_ll_getattr,
	/*.readdir	= main_ll_readdir,
	.open		= main_ll_open,
	.read		= main_ll_read,*/
};

int main(int argc, char *argv[])
{
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
	struct fuse_chan *ch;
	char *mountpoint;
	int err = -1;

	if (fuse_parse_cmdline(&args, &mountpoint, NULL, NULL) != -1 &&
	    (ch = fuse_mount(mountpoint, &args)) != NULL) {
		struct fuse_session *se;

		se = fuse_lowlevel_new(&args, &main_ll_oper,
				       sizeof(main_ll_oper), NULL);
		if (se != NULL) {
			if (fuse_set_signal_handlers(se) != -1) {
				fuse_session_add_chan(se, ch);
				err = fuse_session_loop(se);
				fuse_remove_signal_handlers(se);
				fuse_session_remove_chan(ch);
			}
			fuse_session_destroy(se);
		}
		fuse_unmount(mountpoint, ch);
	}
	fuse_opt_free_args(&args);

	return err ? 1 : 0;

    //return 0;
}