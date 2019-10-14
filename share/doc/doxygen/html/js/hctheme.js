/**
 * Grid-light theme for Highcharts JS
 * @author Torstein Honsi
 */

// Load the fonts
Highcharts.createElement('link', {
    href: 'https://fonts.googleapis.com/css?family=Dosis:400,600',
    rel: 'stylesheet',
    type: 'text/css'
}, null, document.getElementsByTagName('head')[0]);

Highcharts.theme = {
    colors: [
                "#BF4141",
                "#D9BC4A",
                "#4EE662",
                "#96DCFF",
                "#A187E6",
                "#FF579A",
                "#FF8457",
                "#998C5A",
                "#3DB39B",
                "#57B0FF",
                "#9F4EE6",
                "#FF96A4",
                "#996A34",
                "#C6E687",
                "#347799",
                "#30308C",
                "#B33D9B",
                "#FFB347",
                "#779ECB",
                "#996FD6",
                "#77DD77",
                "#FF6961",
                "#DEA5A4",
                "#C2B280",
                "#B19CD9",
                "#AEC6CF",
                "#03C03C",
    ],
    chart: {
        backgroundColor: null,
        style: {
            fontFamily: "Lato"
        }
    },
    title: {
        style: {
            fontSize: '16px',
            fontWeight: 'bold'
        }
    },
    tooltip: {
        borderWidth: 0,
        backgroundColor: 'rgba(219,219,216,0.8)',
        shadow: false
    },
    legend: {
        itemStyle: {
            fontWeight: 'normal',
            fontSize: '14px'
        }
    },
    xAxis: {
        gridLineWidth: 0,
        labels: {
            style: {
                fontSize: '12px'
            }
        }
    },
    yAxis: {
        minorTickInterval: 'auto',
        title: {
            style: {
                fontFamily: "Lato",
                fontSize: '14px'
            }
        },
        labels: {
            style: {
                fontFamily: "Lato",
                fontSize: '12px'
            }
        }
    },
    plotOptions: {
        candlestick: {
            lineColor: '#404048'
        },
        series: {
            shadow: false
        },
        spline: {
            shadow: false,
            dataLabels: {
                shadow: false
            }
        }
    },


    // General
    background2: '#F0F0EA'

};

// Apply the theme
Highcharts.setOptions(Highcharts.theme);