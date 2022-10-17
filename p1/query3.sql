SELECT emp
FROM   employees emp
       JOIN employees em
         ON emp.reportsto = em.employeenumber
       JOIN employees e
         ON em.reportsto = e.employeenumber
WHERE  e.reportsto IS NULL; 
 