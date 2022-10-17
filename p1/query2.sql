SELECT p.productline,
       Avg(orde.shippeddate - orde.orderdate)
FROM   productlines p
       JOIN products pr
         ON pr.productline = p.productline
       JOIN orderdetails ord
         ON ord.productcode = pr.productcode
       JOIN orders orde
         ON orde.ordernumber = ord.ordernumber
GROUP  BY p.productline;