bin_PROGRAMS = sqlite3recover
sqlite3recover_SOURCES = sqlite3recover.cpp cellanalyer.cpp pageanalyer.cpp \
	basetype.cpp dbrecover.cpp sqliteapi.cpp tableanalyer.cpp

# set the include path found by configure
INCLUDES = -I./../../sqlite $(all_includes)

# the library search path.
sqlite3recover_LDFLAGS = $(all_libraries) 
noinst_HEADERS = cellanalyer.h pageanalyer.h basetype.h dbrecover.h sqliteapi.h \
	tableanalyer.h
sqlite3recover_LDADD = -L./../../lib -lsqlite3 -lz
