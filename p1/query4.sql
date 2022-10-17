SELECT of.officecode            AS oficina,
       Sum(ord.quantityordered) AS cantidad
FROM   orderdetails ord
       NATURAL JOIN orders orde
       NATURAL JOIN customers c
       JOIN employees emp
         ON emp.employeenumber = c.salesrepemployeenumber
       JOIN offices of
         ON emp.officecode = of.officecode
GROUP  BY of.officecode
ORDER  BY cantidad DESC
LIMIT  1;