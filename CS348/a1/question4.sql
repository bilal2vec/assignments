select distinct student.snum, student.firstname, student.year from student \
inner join enrollment as e on e.snum = student.snum \
where \
    student.year = 4 and \
    (substr(e.cnum, 1, 2) = 'CS' or substr(e.cnum, 1, 2) = 'CO') and \
    e.grade is not null and \
    not exists ( \
        select * from enrollment as e2 \
        where \
            e2.snum = e.snum and \
            e2.grade is not null and \
            (substr(e2.cnum, 1, 2) = 'CS' or substr(e2.cnum, 1, 2) = 'CO') and \
            e2.grade < 90 \
    )