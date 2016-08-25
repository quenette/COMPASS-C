PWD=`pwd`
SUBDIRS=aspect

all:
	set -e; for dir in ${SUBDIRS} ; do  ${MAKE} -C $$dir all; done

check:
	set -e; for dir in ${SUBDIRS} ; do  ${MAKE} -C $$dir check; done

clean:
	set -e; for dir in ${SUBDIRS} ; do  ${MAKE} -C $$dir clean; done
