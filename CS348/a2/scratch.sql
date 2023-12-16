-- select s.term, s.pnum, s.day, s.time, s.cnum, s.section, 'T' as activity_type from section as s;


-- select o.term, o.pnum, o.day, o.time, o.cnum, null as SECTION, 'O' as activity_type from officehour as o;


-- select s.term, e.snum, s.day, s.time, s.cnum, s.section, 'C' as activity_type from section as s inner join enrollment as e on e.cnum = s.cnum and e.term = s.term and e.section = s.section;

-- select activities.day, activities.time, activities.cnum, activities.section, activities.activity_type, conflicts.n_duplicates from ((select s.term, s.pnum as id, s.day, s.time, s.cnum, s.section, 'A' as activity_type from section as s) union all (select o.term, o.pnum as id, o.day, o.time, o.cnum, null as SECTION, 'B' as activity_type from officehour as o) union all (select s.term, e.snum as id, s.day, s.time, s.cnum, s.section, 'C' as activity_type from section as s inner join enrollment as e on e.cnum = s.cnum and e.term = s.term and e.section = s.section)) as activities, (select activities.term, activities.id, count(*) as n_duplicates, activities.day, activities.time from ((select s.term, s.pnum as id, s.day, s.time, s.cnum, s.section, 'A' as activity_type from section as s) union all (select o.term, o.pnum as id, o.day, o.time, o.cnum, null as SECTION, 'B' as activity_type from officehour as o) union all (select s.term, e.snum as id, s.day, s.time, s.cnum, s.section, 'C' as activity_type from section as s inner join enrollment as e on e.cnum = s.cnum and e.term = s.term and e.section = s.section)) as activities group by activities.term, activities.id, activities.day, activities.time) as conflicts where conflicts.term = activities.term and conflicts.id = activities.id and conflicts.day = activities.day and conflicts.time = activities.time and activities.term = 'S2020' and activities.id = 1 order by activities.activity_type, (case activities.day when 'Monday' then 1 when 'Tuesday' then 2 when 'Wednesday' then 3 when 'Thursday' then 4 when 'Friday' then 5 when 'Saturday' then 6 when 'Sunday' then 7 end), activities.time;

-- select s.day, s.time, s.cnum, s.section, 'T' as activity_type, s2.count from section as s, (select count(*) as count, section.day, section.time from section group by section.day, section.time) as s2 where s.term = 'S2020' and s.pnum = 1 and s2.day = s.day and s2.time = s.time;

-- select officehour.day, officehour.time, officehour.cnum, null, 'O' from  officehour where officehour.term = 'S2020' and officehour.pnum = 1;

-- select section.day, section.time, section.cnum, section.section, 'C' from section inner join enrollment on enrollment.cnum = section.cnum and enrollment.term = section.term and enrollment.section = section.section where section.term = 'S2020' and enrollment.snum = 1;

-- select id from ((select professor.pnum as id from professor) union (select student.snum as id from student)) as ids where ids.id = 1;

-- select section from section where exists (select time, day from section where cnum = 'CS348' and term = (select distinct term from enrollment where grade is null) and section = 6) and section = 5 and (day != 'Tuesday' or time != '11:00:00');

-- select cnum from course where cnum = 'CS348';
-- select year from student where snum = 1;

-- select section from section where pnum = 1 and day = 'Monday' and time = '11:00:00';

-- select pnum from section where cnum = 'CS348' and term = (select distinct term from enrollment where grade is null) and section = 6;

-- select * from section;

-- insert into section (cnum, term, section, pnum, day, time) values ('CS348', (select distinct term from enrollment where grade is null), 8, 1, 'Monday', '11:23:00');

-- update section set pnum = 3, day = 'Monday', time = '12:34:56' where cnum = 'CS348' and term = (select distinct term from enrollment where grade is null) and section = 10;

-- select * from section;

-- select cnum from officehour where cnum = 'CS348' and term = (select distinct term from enrollment where grade is null) and pnum = 1 and day = 'Monday' and time = '10.01.00';

-- select * from officehour;

-- insert into officehour (cnum, term, pnum, day, time) values ('CS348', (select distinct term from enrollment where grade is null), 1, 'Monday', '10:00:00');

-- select * from officehour;

-- select pnum from officehour where pnum = 1 and day = 'Friday' and time = '12.34.00';

-- select cnum from officehour where cnum = 'CS348' and term = (select distinct term from enrollment where grade is null) and pnum = 1 and day = 'Friday' and time = '12.34.00';

-- select cnum+pnum as t from officehour where cnum = 'CS348' and term = (select distinct term from enrollment where grade is null) and pnum = 3 and day = 'Monday' and time = '12.32.00';

select * from section;
select * from enrollment;

-- select distinct snum from enrollment as e inner join section as s on s.cnum = e.cnum and s.term = e.term and s.section = e.section where e.snum = 1 and e.term = (select distinct term from enrollment where grade is null) and s.day = 'Thursday' and s.time = '15.01.00';

-- select * from enrollment where snum = 1 and cnum = 'CS348' and term = (select distinct term from enrollment where grade is null) and section = 3;

-- insert into enrollment (snum, cnum, term, section) values (1, 'CS348', (select distinct term from enrollment where grade is null), 3);