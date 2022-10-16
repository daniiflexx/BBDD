SELECT emp from employees emp join employees em on emp.reportsto=em.employeenumber join employees e on em.reportsto=e.employeenumber
where e.reportsto is NULL;
 