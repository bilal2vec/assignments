select count(student.snum) as n_students, professor.pnum, professor.lastname, section.cnum, section.term, section.section from enrollment \
    left join (select * from student  where (student.year = 3 or student.year = 4)) as student on student.snum = enrollment.snum \
    inner join section on section.cnum = enrollment.cnum and section.term = enrollment.term and section.section = enrollment.section \
    inner join professor on professor.pnum = section.pnum \
    inner join course on course.cnum = section.cnum \
    where \
        grade is not null and \
        professor.dept = 'PM' \
    group by professor.pnum, professor.lastname, section.cnum, section.term, section.section \
    order by professor.lastname, professor.pnum, section.cnum, section.term, section.section