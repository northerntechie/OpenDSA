$(document).ready(function() {
    "use strict";
    var av = new JSAV("FibGraphCON");

    var len = 60;
    var x = 35;
    var bx = 30;
    var hx = 60;
    var fib = [1,1,2,3,5,8,13,21,34]

    av.g.rect(20, 20, 60, 50);
    av.g.rect(20, 5, 60, 15);
    bx = bx + 50;
    av.label("6",
        {top:  -13, left: 45});

    var index = "";
    for (var i = 0; i < 6; i++)
    {
        index = 5 - i;
        // TOP
        av.g.rect(bx, 20, 60, 50);
        // BOTTOM
        av.g.rect(bx, 5, 60, 15);

        av.label(index,
            {top:  -13, left: bx+25});
        bx = bx + 60;
    }

    var lf = 0;
    var p1 = 155;
    var p2 = 65;
    av.g.path("M 155 55 A 150 200 0 0 1 65 55",{"arrow-start": "classic-wide-long"});
    for (var i = 0; i < 7; i++)
    {
        av.g.rect(x, 30, 30, 15);
        av.g.rect(x, 45, 30, 15);
        if(i < 5)
        av.g.line(x +30 , 45 , x + 58,45, {"arrow-end": "classic-wide-long"});
        x = x + len;

        av.label(6-i,
            {top:  12, left: lf + 45});
        // formatting numbers
        if (fib[6-i] > 10)
        av.label(fib[6-i],
            {top:  28, left: lf + 43});
        else
            av.label(fib[6-i],
                {top:  28, left: lf + 45});

        lf = lf + 60;
        // set up param for path
        var pathParam = "M ";
        p1 = p1 + 60;
        pathParam += p1.toString();
        var p = " 55 A 150 200 0 0 1 ";
        pathParam += p;
        p2 = p2 + 60;
        pathParam += p2.toString();
        pathParam += " 55";
        // Double curved arrow
        if(i < 4)
        av.g.path(pathParam,{"arrow-start": "classic-wide-long"} );
    }
    av.displayInit();
    av.recorded();
});
