MAKE=make
RM=rm
SUBDIRS=calendar contacts

all:
	list='$(SUBDIRS)'; \
	for subdir in $$list; do \
      echo "Making $$target in $$subdir"; \
      (cd $$subdir && $(MAKE)) \
	done;

clean:
	rm *.so*
	list='$(SUBDIRS)'; \
	for subdir in $$list; do \
      echo "Making $$target in $$subdir"; \
      (cd $$subdir && $(MAKE) clean) \
	done;

