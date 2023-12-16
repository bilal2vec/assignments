select distinct min_grade.min_grade, max_grade.max_grade, professor.lastname, course.cnum, s.term, s.section  from section as s, \
    ( \
        select e.grade as min_grade, e.cnum, e.term, e.section from enrollment as e \
            where not exists \
                (select e2.grade from enrollment as e2 \
                    where e2.grade < e.grade and e2.cnum = e.cnum and e2.term = e.term and e2.section = e.section \
                ) \
    ) as min_grade, \
    ( \
        select e.grade as max_grade, e.cnum, e.term, e.section from enrollment as e \
            where not exists \
                (select e2.grade from enrollment as e2 \
                    where e2.grade > e.grade and e2.cnum = e.cnum and e2.term = e.term and e2.section = e.section \
                ) \
    ) as max_grade \
    inner join enrollment on \
        enrollment.cnum = s.cnum and \
        enrollment.term = s.term and \
        enrollment.section = s.section \
    inner join professor on \
        professor.pnum = s.pnum \
    inner join course on \
        course.cnum = s.cnum \
    where \
        enrollment.grade is not null and \
        enrollment.cnum = min_grade.cnum and enrollment.term = min_grade.term and enrollment.section = min_grade.section and \
        enrollment.cnum = max_grade.cnum and enrollment.term = max_grade.term and enrollment.section = max_grade.section and \
        professor.dept = 'CS' and \
        course.cnum in (select cnum from officehour where officehour.day = 'Monday' and officehour.cnum = course.cnum and officehour.pnum = professor.pnum) and \
        course.cnum in (select cnum from officehour where officehour.day = 'Friday' and officehour.cnum = course.cnum and officehour.pnum = professor.pnum) 