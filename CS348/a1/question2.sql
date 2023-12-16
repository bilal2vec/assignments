select distinct professor.pnum, professor.lastname from professor \
	where professor.pnum not in ( \
		select distinct section.pnum from section \
			inner join enrollment on \
				section.cnum = enrollment.cnum and \
				section.term = enrollment.term and \
				section.section = enrollment.section \
			where \
				(section.cnum = 'CS348' or section.cnum = 'CS234') and enrollment.grade is not null \
	) and professor.pnum in ( \
		( \
			select distinct section.pnum from section \
				inner join enrollment on \
					section.cnum = enrollment.cnum and \
					section.term = enrollment.term and \
					section.section = enrollment.section \
				where \
					(section.cnum = 'CS348') and enrollment.grade is null \
		) union ( \
			select distinct section.pnum from section \
				inner join enrollment on \
					section.cnum = enrollment.cnum and \
					section.term = enrollment.term and \
					section.section = enrollment.section \
				where \
					(section.cnum = 'CS234') and enrollment.grade is null \
		) \
	) and professor.pnum not in ( \
		( \
			select distinct section.pnum from section \
				inner join enrollment on \
					section.cnum = enrollment.cnum and \
					section.term = enrollment.term and \
					section.section = enrollment.section \
				where \
					(section.cnum = 'CS348') and enrollment.grade is null \
		) intersect ( \
			select distinct section.pnum from section \
				inner join enrollment on \
					section.cnum = enrollment.cnum and \
					section.term = enrollment.term and \
					section.section = enrollment.section \
				where \
					(section.cnum = 'CS234') and enrollment.grade is null \
		) \
	) \
	and professor.dept = 'CS'