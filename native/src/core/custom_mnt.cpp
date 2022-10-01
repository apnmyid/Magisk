#include <stdio.h>
#include <sys/mount.h>
#include <string>

#include <base.hpp>
#include <magisk.hpp>

static long mnt_getflags(const char* flags){
    char buf[4098];
    char flags_[4098];
    long nflags=0;
    sprintf(flags_, "%s,", flags);
    int n=0;
    for (int i=0; flags_[i]; i++) {
        if (flags_[i] == ','){
            buf[n]='\0';
            if (strcmp(buf, "bind")==0)
                nflags |= MS_BIND;
            else if (strcmp(buf, "remount")==0)
                nflags |= MS_REMOUNT;
            else if (strcmp(buf, "rec")==0)
                nflags |= MS_REC;
            else if (strcmp(buf, "move")==0)
                nflags |= MS_MOVE;
            else if (strcmp(buf, "dirsync")==0)
                nflags |= MS_DIRSYNC;
            else if (strcmp(buf, "lazytime")==0)
                nflags |= MS_LAZYTIME;
            else if (strcmp(buf, "mandlock")==0)
                nflags |= MS_MANDLOCK;
            else if (strcmp(buf, "noatime")==0)
                nflags |= MS_NOATIME;
            else if (strcmp(buf, "nodev")==0)
                nflags |= MS_NODEV;
            else if (strcmp(buf, "nodiratime")==0)
                nflags |= MS_NODIRATIME;
            else if (strcmp(buf, "noexec")==0)
                nflags |= MS_NOEXEC;
            else if (strcmp(buf, "nosuid")==0)
                nflags |= MS_NOSUID;
            else if (strcmp(buf, "ro")==0)
                nflags |= MS_RDONLY;
            else if (strcmp(buf, "relatime")==0)
                nflags |= MS_RELATIME;
            else if (strcmp(buf, "silent")==0)
                nflags |= MS_SILENT;
            else if (strcmp(buf, "strictatime"))
                nflags |= MS_STRICTATIME;
            else if (strcmp(buf, "sync"))
                nflags |= MS_SYNCHRONOUS;
            n=0;
        } else {
            buf[n]=flags_[i];
            n++;
        }
    }
    return nflags;
}

int load_mount(const char* filename)
{
    char *source;
    char *target;
    char *filesystem;
    char *flags;
    char *data;
    char buf[1024];
    FILE *f = fopen(filename, "r");
    if (!f) return -1;
    LOGD("dyn_mount: load %s\n", filename);
    while (!feof(f)){
        source=new char[1024];
        target=new char[1024];
        data=new char[1024];
        filesystem=new char[1024];
        flags=new char[1024];
        fscanf(f, "%s %s %s %s %s",
            source,
            target,
            filesystem,
            flags,
            data);
        if (data && strcmp(data, "0")==0) {
            delete data;
            data=nullptr;
        }
        if (source && strcmp(source, "0")==0) {
            delete source;
            source=nullptr;
        }
        if (target && strcmp(target, "0")==0) {
            delete target;
            target=nullptr;
        }
        if (filesystem && strcmp(filesystem, "0")==0) {
            delete filesystem;
            filesystem=nullptr;
        }
        if (strlen(target)>0) {
            LOGD("dyn_mount: %s -> %s fs=[%s] flags=[%s] option=[%s]\n", source, target, filesystem, flags, data);
            xmount(source, target, filesystem, mnt_getflags(flags), data);
        }
        if (data) delete data;
        if (source) delete source;
        if (target) delete target;
        if (filesystem) delete filesystem;
        if (flags) delete flags;
    }
    fclose(f);
    return 0;
}
