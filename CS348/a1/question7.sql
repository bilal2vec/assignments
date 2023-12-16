select cast((cast(count(distinct prev_terms.pnum) as float) / cast((select count(pnum) from professor) as float))*100 as int) as percentage from \
( \
    select count(section.section) as num_sections, section.cnum, section.term, section.pnum  from section \
        inner join enrollment on \
            enrollment.cnum = section.cnum and \
            enrollment.term = section.term and \
            enrollment.section = section.section \
        where \
            enrollment.grade is not null \
        group by section.cnum, section.term, section.pnum \
) as prev_terms left join \
( \
    select count(section.section) as num_sections, section.cnum, section.term, section.pnum  from section \
        inner join enrollment on \
            enrollment.cnum = section.cnum and \
            enrollment.term = section.term and \
            enrollment.section = section.section \
        where \
            enrollment.grade is null \
        group by section.cnum, section.term, section.pnum \
) as curr_terms on prev_terms.cnum = curr_terms.cnum and prev_terms.pnum = curr_terms.pnum \
where \
    prev_terms.num_sections >= 2 and \
    (curr_terms.num_sections is null or curr_terms.num_sections < 2)
