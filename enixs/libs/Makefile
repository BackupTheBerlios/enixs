MAKE=make
RM=rm
SUBDIRS=userdata util

all:
	list='$(SUBDIRS)'; \
	for subdir in $$list; do \
      echo "Making $$target in $$subdir"; \
      (cd $$subdir && $(MAKE) install) \
	done;

clean:
	rm *.so*
	list='$(SUBDIRS)'; \
	for subdir in $$list; do \
      echo "Making $$target in $$subdir"; \
      (cd $$subdir && $(MAKE) clean) \
	done;

