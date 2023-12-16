select distinct professor.pnum, professor.lastname from enrollment \
    inner join section on section.cnum = enrollment.cnum and section.term = enrollment.term and section.section = enrollment.section \
    inner join professor on professor.pnum = section.pnum \
    where enrollment.grade <= all ( \
        select enrollment.grade from enrollment \
            where enrollment.cnum = 'CS245' and \
            enrollment.grade is not null \
        ) \
        and enrollment.cnum = 'CS245' \
        and enrollment.grade is not null