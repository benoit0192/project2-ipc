CS551 - Project 2 - MINIX3 IPC
==============================

OS Design & Implementation

Illinois Institute of Technology

March 2017

TODO
----
syscall wrappers are located in `src/minix/lib/libc/sys/topic.c`

syscall implementations are located in `src/minix/servers/pm/topic.c`

* pass buffer and size arguments correctly in syscall wrappers and return result in allocated buffer for `topic_lookup()`, `topic_publish()` and `topic_retrieve()`. Use memory grant? Or maybe `sys_memcopy()`?
* implement all syscalls. for now they just print their names.
* install/generate man pages from Benoit's `doc/man`

Project members
---------------

Benoit Lafon

Yi Qu

Sylvain Cassiau

How to setup your development environment
-----------------------------------------

This method uses virtualbox to virtualize MINIX.

The first thing to do is to clone the git repository on your computer. Let's call `REPO` the path to your local copy of the repository.

You now need to install a Minix 3 virtual machine in virtualbox.
You can download the Minix .iso from here: http://wiki.minix3.org/doku.php?id=www:download:start
The following Minix wiki page gives easy step by step setup instructions: http://wiki.minix3.org/doku.php?id=usersguide:runningonvirtualbox

The next step consists in the creation of a shared folder between the VM and your host OS.
The last section of the wiki page explains how to do so, but here are more specific instructions to get it work with the scripts included in the project's source code:

* shut down the VM;
* open the VM configuration window, and in the shared folder tab, add a new shared folder. When asked for its name, type "share" and use `REPO` as the local path;
* boot the VM
* login as root
* type the following command: `mkdir -p /mnt/shared && mount -t vbfs -o share=share none /mnt/shared`

The content of the git repository located on your computer is now available from the VM in the directory `/mnt/shared`. Unfortunately, the file system implementation used to create shared folder in Minix do not track file modification time correctly. Therefore clang won't compile correctly in this directory and makefile behaviors are erratic. A solution is to make a copy of the shared folder in the VM before compiling. A script is included in the project's source code in order to simplify this task. Run this command to install useful command aliases (and get a better shell prompt): `rm /root/.profile && cp /mnt/shared/src/etc/ast/.profile /root/.profile && mkdir -p /home/repo`. To load this new file you need to logout (press Ctrl+D on an empty command line) or reboot (run `reboot`) and login again as root.

Your new `.profile` file does a few things:

* it displays the current working directory in the shell prompt;
* it sets your home directory to `/home`;
* it automatically mount the shared folder in `/mnt/shared` when you log in;
* it defines the `ossync` command, which synchronizes the contents of the shared folder with the `/home/repo` local copy of the git repository;
* it defines the `recomp` command, which recompiles Minix sources located in `/home/repo` and install it for the next reboot.

Starting from now, you can work on your PC, using your favorite source code editor.
When you want to apply your modifications in Minix, you simply need to run `ossync` in the VM to update the VM source code copy (it takes some time since the code is huge), and then `recomp`.

Git branching model
-------------------

The `master` branch is the main reference branch. The source code in the `master` branch must always work correctly. Therefore, do not commit untested code to it.

When you start working on a feature or start modifying code, you first need to create a new local branch for your work. To do so, run `git checkout -b my-feature-branch` where my-feature-branch is the name you have chosen for your branch ;-) .

You are allowed to do everything that you want in your branch.

Once your modifications are complete, test your code.

When everything is working, you can review the list of your changes with `git status`.

You need to add all the modified source files (no binary file) that you want to save to the repository with `git add /here/there/modified_file_1 /dir/subdir/modified_file_2`...

You can now commit your changes with `git commit -m "a short message describing your changes"`. This will save your changes to your local copy of the repository.

To add your modifications to the `master` branch, first go back to it: `git checkout master`, then update its contents with the other users modifications: `git pull`, merge your branch onto it `git merge my-feature-branch`. Don't forget that code in the `master` branch must always work correctly. And finally delete your local working branch: `git branch -d my-feature-branch`.
