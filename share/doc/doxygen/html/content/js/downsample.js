$(function(){

    var floor = Math.floor,
        abs = Math.abs;

    var downsampleAmount = {
        'HeartRate(1/min)': 0.01,
        'HeartStrokeVolume(mL)': 0.03,
        'ExtravascularFluidVolume(L)': 0.02,
        'BloodVolume(mL)': 0.001,
        'ArterialPressure(mmHg)': 0.1,
        'MeanArterialPressure(mmHg)': 0.05,
        'SystolicArterialPressure(mmHg)': 0.03,
        'DiastolicArterialPressure(mmHg)': 0.03,
        'CardiacOutput(L/min)': 0.03,
        'HemoglobinContent(g)': 0.001,
        'CentralVenousPressure(mmHg)': 0.02,
        'PulmonaryCapillariesWedgePressure(mmHg)': 0.01,
        'RespirationRate(1/min)': 0.01,
        'OxygenSaturation': 0.01,
        'OxygenConsumptionRate(mL/min)': 0.01,
        'TidalVolume(mL)': 0.01,
        'TotalLungVolume(L)': 0.02,
        'EndTidalCarbonDioxideFraction': 0.01,
        'Aorta-Oxygen-PartialPressure(mmHg)': 0.15,
        'Aorta-CarbonDioxide-PartialPressure(mmHg)': 0.15,
        'Trachea-Oxygen-PartialPressure(cmH2O)': 0.02,
        'Trachea-CarbonDioxide-PartialPressure(cmH2O)': 0.02,
        'Lead3ElectricPotential(mV)': 0.5,
        'TotalMetabolicRate(kcal/day)': 0.01,
        'SkinTemperature(degC)': 0.01,
        'CoreTemperature(degC)': 0.01,
        'CarbonDioxideProductionRate(mL/min)': 0.01,
        'SweatRate(mg/min)': 0.01,
        'TotalWorkRateLevel': 0.01,
        'FatigueLevel': 0.01,
        'AchievedExerciseLevel': 0.01,
        'BloodPH': 0.01

        //'AirwayOxygenPartialPressure(cmH2O)': 0.01,
        //'AirwayCarbonDioxidePartialPressure(cmH2O)': 0.01,
        //'InspiredTidalVolume(mL)': 0.01,
        //'ExpiredTidalVolume(L)': 0.01
    };

    //$.each(downsampleAmount, function(key, value){
    //
    //    $('#fidelity > div.panel-body').append('<div class="form-group">'+
    //                                                '<label>'+key+'</label>' +
    //                                                '<input type="number" style="width: 200px" class="form-control" value="'+value*100+'">'+
    //                                            '</div>');
    //
    //});

    function addDownSample(key, value) {
        $('#fidelity > div.panel-body').append('<div class="form-group">'+
                                                    '<label>'+key+'</label>' +
                                                    '<input type="number" style="width: 200px" class="form-control" value="'+value*100+'">'+
                                                '</div>');
    }

    function largestTriangleThreeBuckets(data, threshold) {

        var data_length = data.length;
        if (threshold >= data_length || threshold === 0) {
            return data; // Nothing to do
        }

        var sampled = [],
            sampled_index = 0;

        // Bucket size. Leave room for start and end data points
        var every = (data_length - 2) / (threshold - 2);

        var a = 0,  // Initially a is the first point in the triangle
            max_area_point,
            max_area,
            area,
            next_a;

        sampled[ sampled_index++ ] = data[ a ]; // Always add the first point

        for (var i = 0; i < threshold - 2; i++) {

            // Calculate point average for next bucket (containing c)
            var avg_x = 0,
                avg_y = 0,
                avg_range_start  = floor( ( i + 1 ) * every ) + 1,
                avg_range_end    = floor( ( i + 2 ) * every ) + 1;
            avg_range_end = avg_range_end < data_length ? avg_range_end : data_length;

            var avg_range_length = avg_range_end - avg_range_start;

            for ( ; avg_range_start<avg_range_end; avg_range_start++ ) {
                avg_x += data[ avg_range_start ][ 0 ] * 1; // * 1 enforces Number (value may be Date)
                avg_y += data[ avg_range_start ][ 1 ] * 1;
            }
            avg_x /= avg_range_length;
            avg_y /= avg_range_length;

            // Get the range for this bucket
            var range_offs = floor( (i + 0) * every ) + 1,
                range_to   = floor( (i + 1) * every ) + 1;

            // Point a
            var point_a_x = data[ a ][ 0 ] * 1, // enforce Number (value may be Date)
                point_a_y = data[ a ][ 1 ] * 1;

            max_area = area = -1;

            for ( ; range_offs < range_to; range_offs++ ) {
                // Calculate triangle area over three buckets
                area = abs( ( point_a_x - avg_x ) * ( data[ range_offs ][ 1 ] - point_a_y ) -
                    ( point_a_x - data[ range_offs ][ 0 ] ) * ( avg_y - point_a_y )
                ) * 0.5;
                if ( area > max_area ) {
                    max_area = area;
                    max_area_point = data[ range_offs ];
                    next_a = range_offs; // Next a is this b
                }
            }

            sampled[ sampled_index++ ] = max_area_point; // Pick this point from the bucket
            a = next_a; // This a is the next a (chosen b)
        }

        sampled[ sampled_index++ ] = data[ data_length - 1 ]; // Always add last

        return sampled;
    }

    function allignToSecond(data)
    {
        time = data[data.length-1][0];

        aligned = [];

        for(var second = 0; second < time; second++)
        {
            dataLeftIndex = 0;
            dataRightIndex = 1;
            //Find Left
            dataIndex = 0;
            while (dataIndex < data.length && second > data[dataIndex][0]) {
                dataIndex++;
            }
            dataLeftIndex = dataIndex;
            //Rind right
            dataIndex = data.length - 1;
            while (dataIndex > 1 && second < data[dataIndex][0]) {
                dataIndex--;
            }
            dataRightIndex = dataIndex;
            //calc line
            slope = (data[dataRightIndex][1] - data[dataLeftIndex][1]) / (data[dataRightIndex][0] - data[dataLeftIndex][0])
            ycept = data[dataLeftIndex][1] - (slope * data[dataLeftIndex][0]);
            aligned.push([second, (slope * second) + ycept]);
        }
        console.log(aligned);
        return aligned;
    }

    var reader;
    var progress = document.querySelector('.percent');

    function abortRead() {
        reader.abort();
    }

    function errorHandler(evt) {
        switch(evt.target.error.code) {
            case evt.target.error.NOT_FOUND_ERR:
                alert('File Not Found!');
                break;
            case evt.target.error.NOT_READABLE_ERR:
                alert('File is not readable');
                break;
            case evt.target.error.ABORT_ERR:
                break; // noop
            default:
                alert('An error occurred reading this file.');
        };
    }

    function updateProgress(evt) {
        // evt is an ProgressEvent.
        if (evt.lengthComputable) {
            var percentLoaded = Math.round((evt.loaded / evt.total) * 100);
            // Increase the progress bar length.
            if (percentLoaded < 100) {
                progress.style.width = percentLoaded + '%';
                progress.textContent = percentLoaded + '%';
            }
        }
    }

    function handleFileSelect(evt) {
        // Reset progress indicator on new file selection.
        progress.style.width = '0%';
        progress.textContent = '0%';

        reader = new FileReader();
        reader.onerror = errorHandler;
        reader.onprogress = updateProgress;
        reader.onabort = function(e) {
            alert('File read cancelled');
        };
        reader.onloadstart = function(e) {
            document.getElementById('progress_bar').className = 'loading';
        };
        reader.onload = function(e) {
            // Ensure that the progress bar displays 100% at the end.
            parseDataFile(reader.result);
            progress.style.width = '100%';
            progress.textContent = '100%';
            setTimeout("document.getElementById('progress_bar').className='';", 2000);
        }

        // Read in the image file as a binary string.
        reader.readAsText(evt.target.files[0]);
    }

    function parseDataFile(file)
    {
        var lines = file.split("\n");
        var headers = lines[0].split(",");
        var splices = [];
        var series = {};
        var downsample = {};
        var downsampleSecond = {};
        headers.splice(0,1);
        for(var h in headers){
            d = Number(h);
            if(headers[h] == "") {
                splices.push(h);
            }
            //headers[h] = headers[h].replace(/([A-Z].[^A-Z(]+)/g, ' $1').trim();
            headers[h] = headers[h].trim();

            if(headers[h].length > 0) {
                if (downsampleAmount.hasOwnProperty(headers[h]))
                    addDownSample(headers[h], downsampleAmount[headers[h]]);
                else
                    addDownSample(headers[h], .2);
            }

            series[headers[h]] = [];
        }
        lines.splice(0,1);
        console.log();
        for(var i in lines){
            var line = lines[i].split(",");
            var time = Number(line.splice(0,1)[0]);
            for(var d in line){
                d = Number(d);
                data = Number(line[d]);
                if(!isNaN(data))
                    series[headers[d]].push([time, data]);
            }
        }
        delete series[""];

        sampleCount = 0;
        $('#data-sample').empty();
        for (var key in series) {
            //downsample[key] = largestTriangleThreeBuckets(series[key], series[key].length);
            downsample[key] = largestTriangleThreeBuckets(series[key], floor(downsampleAmount[key] * series[key].length));
            //downsampleSecond[key] = allignToSecond(downsample[key]);
            //$('#data-sample').append("<h5>" + key + "</h5>");
            //$('#data-sample').append("<div id='downsample"+sampleCount+"' style='width: 100%; height: 600px;'></div><hr>");
            //$('#data-sample').append("<div id='downsampleSecond"+sampleCount+"' style='width: 1000px; height: 600px'></div>");
            //$.plot("#downsample"+sampleCount, [series[key].slice(0, 1000)]);
            //$.plot("#downsample"+sampleCount, [downsample[key]]);
            //$.plot("#downsampleSecond"+sampleCount, [downsampleSecond[key]]);
            sampleCount++;
        }

        $('#datafile').val(JSON.stringify(downsample));

    }

    document.getElementById('files').addEventListener('change', handleFileSelect, false);

});