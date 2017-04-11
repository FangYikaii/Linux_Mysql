CREATE TABLE children
(
    childno int(11) NOT NULL auto_increment,
    fname varchar(30),
    age int(11),
    PRIMARY KEY (childno)
);

INSERT INTO children(childno,fname,age) VALUE (1,'Jenny',21);
INSERT INTO children(childno,fname,age) VALUE (2,'Andrew',22);
INSERT INTO children(childno,fname,age) VALUE (3,'Gavin',23);
INSERT INTO children(childno,fname,age) VALUE (4,'Russ',24);
INSERT INTO children(childno,fname,age) VALUE (5,'Adrian',25);
