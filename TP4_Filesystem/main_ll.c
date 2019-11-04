#include <fuse_lowlevel.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void main_ll_lookup(fuse_req_t req, fuse_ino_t parent, const char *name) {

}

static void main_ll_getattr(fuse_req_t req, fuse_ino_t ino,
			     struct fuse_file_info *fi) {
	struct stat stbuf;

	(void) fi;

	memset(&stbuf, 0, sizeof(stbuf));
	if (hello_stat(ino, &stbuf) == -1)
		fuse_reply_err(req, ENOENT);
	else
		fuse_reply_attr(req, &stbuf, 1.0);
}

static void main_ll_readdir(fuse_req_t req, fuse_ino_t ino, size_t size,
			     off_t off, struct fuse_file_info *fi) {

}

static void main_ll_open(fuse_req_t req, fuse_ino_t ino,
			  struct fuse_file_info *fi) {

}

static void main_ll_read(fuse_req_t req, fuse_ino_t ino, size_t size,
			  off_t off, struct fuse_file_info *fi) {

}

static struct fuse_lowlevel_ops main_ll_oper = {
	.lookup		= main_ll_lookup,
	.getattr	= main_ll_getattr,
	.readdir	= main_ll_readdir,
	.open		= main_ll_open,
	.read		= main_ll_read,
};

int main()
{
	/*struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
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

	return err ? 1 : 0;*/

    return 0;
}