MAKE=make
RM=rm
SUBDIRS=db/driver/sapdb libs plugins enixs

all:
	list='$(SUBDIRS)'; \
	for subdir in $$list; do \
      echo "Making $$target in $$subdir"; \
      (cd $$subdir && $(MAKE) all) \
	done;

clean:
	list='$(SUBDIRS)'; \
	for subdir in $$list; do \
      echo "Making $$target in $$subdir"; \
      (cd $$subdir && $(MAKE) clean) \
	done;

tar:
	rm -f backup.tar.gz
	tar cf backup.tar *
	tar --delete -f backup.tar *.o
	gzip -9 backup.tar

dist:
	mkdir tmp
	mkdir tmp/enixs
	mkdir tmp/enixs/bin
	mkdir tmp/enixs/icons
	mkdir tmp/enixs/lib
	mkdir tmp/enixs/plugins
	mkdir tmp/enixs/translations
	cp enixs/enixs tmp/enixs/bin
	cp enixs/enixs.png tmp/enixs/icons
	cp enixs/*.qm tmp/enixs/translations
	cp -d libs/libenixs* tmp/enixs/lib
	cp plugins/*.so tmp/enixs/plugins
	list=`ls plugins`; \
	for i in $$list; do \
		if [ -d plugins/$$i ]; \
		then \
			cp plugins/$$i/*.qm tmp/enixs/translations 2> /dev/null; \
		fi \
	done;
	(cd tmp && tar czf ../enixs.tar.gz enixs)
	rm -rf tmp

dist-save:
	cp enixs.tar.gz /server/software/enixs
	cp install.sh /server/software/enixs

