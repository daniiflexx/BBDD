WITH total
     AS (SELECT c.customernumber AS cnb,
                c.customername   AS cna
         FROM   orderdetails ord
                JOIN products pr
                  ON ord.productcode = pr.productcode
                JOIN orders o
                  ON ord.ordernumber = o.ordernumber
                JOIN customers c
                  ON o.customernumber = c.customernumber
         WHERE  pr.productname = '1940 Ford Pickup Truck'
         GROUP  BY c.customernumber)
SELECT total.cnb,
       total.cna,
       Sum(p.amount) suma
FROM   total
       JOIN payments p
         ON p.customernumber = total.cnb
GROUP  BY total.cnb,
          total.cna
ORDER  BY suma DESC;