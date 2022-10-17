WITH total
     AS (SELECT officecode AS offi,
                country    AS pais
         FROM   offices
         GROUP  BY officecode
         EXCEPT
         SELECT OF.officecode,
                OF.country
         FROM   employees emp
                JOIN customers c
                  ON c.salesrepemployeenumber = emp.employeenumber
                natural JOIN orders ord
                JOIN offices OF
                  ON emp.officecode = OF.officecode
         WHERE  ord.orderdate BETWEEN '2003-01-01' AND '2003-12-31'
         GROUP  BY OF.officecode)
SELECT total.pais,
       Count(total.offi) AS oficinas
FROM   total
GROUP  BY total.pais
ORDER  BY oficinas DESC; 