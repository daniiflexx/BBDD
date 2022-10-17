WITH total
     AS (SELECT ord.productcode,
                orde.productcode,
                Count(ord.ordernumber) AS cestas
         FROM   orderdetails ord
                JOIN orderdetails orde
                  ON ord.ordernumber = orde.ordernumber
         WHERE  ord.productcode > orde.productcode
         GROUP  BY ord.productcode,
                   orde.productcode)
SELECT *
FROM   total
WHERE  cestas > 1;