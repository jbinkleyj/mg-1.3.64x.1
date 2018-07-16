#! /bin/sh
# Convert types long and u_long in the mg-1.3x source tree
# to MG_long_t and MG_u_long_t.
# [30-Oct-2004] -- update with change to distinctive type names.
# [26-May-1997]
for f in `find . -name '*.[chyl]' -print | sort`
do
	mv $f $f.old
	sed	\
		-e 's/u_long/MG_u_long_t/g' \
		-e 's/[(]long[)]/ (MG_long_t) /g' \
		-e 's/[(]long[ 	][ 	]*/ (MG_long_t /g' \
		-e 's/[(]unsigned[ 	][ 	]*long[)]/ (unsigned MG_long_t) /g' \
		-e 's/[(]unsigned[ 	][ 	]*long[ 	][ 	]*int[)]/ (unsigned MG_long_t) /g' \
		-e 's/^long[ 	]int[ 	][ 	]*/MG_long_t /g' \
		-e 's/^long/MG_long_t/g' \
		-e 's/^\\^MG_long_t/MG_long_t/g' \
		-e 's/[ 	]long[ 	]int[ 	]/ MG_long_t /g' \
		-e 's/[ 	]long[ 	]/ MG_long_t /g' \
		-e 's/[ 	]long:/ MG_long_t : /g' \
		-e 's/[ 	]long$/ MG_long_t/g' \
		<$f.old >$f
	rm -f $f.old
	echo $f
done
