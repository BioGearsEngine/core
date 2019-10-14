/**
 * BioGears Showcase Player
 * showcase.js
 * http://www.ara.com
 *
 * Copyright 2014, Applied Research Associates
 * Developed under the U.S. Government BioGears research program
 *
 * Written by Anthony Hamilton
 *
 * Contact admin@biogearsengine.com if you wish to use this code on your website
 */

;(function ($, undefined) {

    /*
     * showcase object.
     */
    $.showcase = function (options, element) {

        this.$showcase = $(element);

        this._init(options);

    };

    $.showcase.defaults = {
        css: '',
        seeMore: false
    };

    $.showcase.prototype = {
        _init: function (options) {

            this.options = $.extend(true, {}, $.showcase.defaults, options);

            this.showcaseId = this.$showcase.attr("data-showcase");
            this.firstLoaded = false;
            this.series = [];
            this.yaxis = [];
            this.timeline = [];
            this.yaxisRight = false;
            this.paused = true;
            this.origonalZoomMin = -1;
            this.origonalZoomMax = -1;
            this.pointMax = 2000;
            this.highcharts = {};

            this._loadDataFile();

            this._loadLogFile();

            this._loadEvents();

        },
        _loadDataFile: function() {

            var _self = this;
            $.getJSON("/showcases/"+this.showcaseId+"/datafile.json")
                .done(function( json ) {
                    _self.data = json;
                    _self.charts = Object.keys(_self.data);
                    _self.max = _self.data[_self.charts[0]][_self.data[_self.charts[0]].length - 1][0];
                    _self.min = _self.data[_self.charts[0]][0][0];

                    if(_self.firstLoaded){
                        _self._loadPlayer();
                    } else {
                        _self.firstLoaded = true;
                    }
                })
                .fail(function( jqxhr, textStatus, error ) {
                    var err = textStatus + ", " + error;
                    console.log( "Request Failed: " + err );
                });

        },
        _loadLogFile: function () {

            var _self = this;
            $.getJSON("/showcases/"+this.showcaseId+"/logfile.json")
                .done(function( json ) {
                    _self.log = json;

                    if(_self.firstLoaded){
                        _self._loadPlayer();
                    } else {
                        _self.firstLoaded = true;
                    }
                })
                .fail(function( jqxhr, textStatus, error ) {
                    var err = textStatus + ", " + error;
                    console.log( "Request Failed: " + err );
                });

        },
        _loadPlayer: function () {

            var _self = this;

            //Data File
            _self.seriesShow = true;
            _self.iterator = 0;
            $.each(_self.data, function(key, value) {

                if(_self.iterator > 3)
                    _self.seriesShow = false;

                if(_self.yaxisRight){
                    _self.px = 5;
                    _self.mx = 10;
                } else {
                    _self.px = -5;
                    _self.mx = 10;
                }

                var max = value[0][1];
                var min = value[0][1];
                $.each(value, function(key, coordinate) {

                    if(coordinate[1] > max)
                        max = coordinate[1];
                    if(coordinate[1] < min)
                        min = coordinate[1];

                });

                _self.yaxis.push({
                    id: key,
                    labels: {
                        style: {
                            color: Highcharts.getOptions().colors[_self.iterator]
                        },
                        x: _self.px
                    },
                    title: {
                        text: key,
                        style: {
                            color: Highcharts.getOptions().colors[_self.iterator]
                        },
                        margin: _self.mx
                    },
                    events: {
                        afterSetExtremes: function(){
                            this.update({
                               visible: false
                            });
                        }
                    },
                    //opposite: false,
                    gridLineWidth: 0,
                    opposite: _self.yaxisRight,
                    showEmpty: false,
                    globalDataMin: min,
                    globalDataMax: max,
                    pointsPerSecond: value.length / (_self.max - _self.min)
                });

                if(_self.seriesShow) {
                    _self.yaxis[_self.iterator].min = min;
                    _self.yaxis[_self.iterator].max = max;
                }

                _self.series.push({
                    name: key,
                    type: 'spline',
                    yAxis: _self.iterator,
                    data: value,
                    visible: _self.seriesShow,
                    marker: {
                        enabled: false
                    }
                });

                _self.yaxisRight = !_self.yaxisRight;
                _self.iterator++;
            });

            //Log File
            $.each(_self.log, function(name, type) {

                var data = [];
                $.each(type, function(second, entry) {


                    var x = second;

                    $.each(entry, function(title, details) {

                        var text = "";

                        if(details.hasOwnProperty('data')){
                            $.each(details.data, function(key, datapoint) {

                                text += "" + datapoint.label + ": " + datapoint.value + "<br>";

                            });
                        } else{
                            text = "";
                        }

                        data.push({
                            x: x,
                            title: title,
                            text: text
                        });


                        //var logtitle = '<h4 class="thin">'+title+' ('+_self._toMMSS(x)+' min)</h4>';
                        //var logdetails = '<p>'+text+'</p>';
                        //var logentry = '<li data-position="'+x+'" class="list-group-item">'+logtitle+logdetails+'</li>';
                        //_self.$showcase.find('[data-log="'+name+'"]').append(logentry);
                        _self._pushToTimeLine(name, x, title, text);

                    });

                });

                _self.yaxis.push({
                    id: name,
                    labels: {
                        enabled: false
                    },
                    title: {
                        text: null
                    }
                });

                _self.series.push({
                    type: 'flags',
                    name: name,
                    color: _self._logSeriesColor(name),
                    data: data,
                    yAxis: _self.iterator,
                    shape: 'squarepin',
                    style : {// text style
                        fontWeight: 'normal'
                    }
                });
                _self.iterator++;
            });

            if(!this.$showcase.hasClass('active')){
                this.$showcase.find('#spinner' + this.showcaseId).hide();
                this.$showcase.find('#tab' + this.showcaseId).removeClass('hide');
            }

            _self._addChart();

            _self._sortTimeLine();

            _self._addTimeLine();

            _self._showShowcase();

        },
        _showShowcase: function () {

            if(this.$showcase.hasClass('active')){

                var _self = this;

                this.$showcase.find('#spinner' + this.showcaseId).fadeOut(function(){

                    _self.$showcase.find('#tab' + _self.showcaseId).hide().removeClass('hide').fadeIn();

                    _self.$showcase.find('.timeline-row').removeClass('active');

                });

            }

        },
        _loadEvents: function () {

            var _self = this;

            this.$showcase.find("[data-player='play']").on('click', function () {

                _self._eventPlay($(this));

            });

            this.$showcase.find("[data-player='pause']").on('click', function () {

                _self._eventPause($(this));

            });

            this.$showcase.find("[data-player='forward']").on('click', function () {

                _self._eventForward($(this));

            });

            this.$showcase.find("[data-player='backward']").on('click', function () {

                _self._eventBackward($(this));

            });

            this.$showcase.find("[data-player='lock']").on('click', function () {

                if(!$(this).hasClass('active'))
                    _self._eventLock();

            });

            this.$showcase.find("[data-player='unlock']").on('click', function () {

                if(!$(this).hasClass('active'))
                    _self._eventUnlock();

            });

            this.$showcase.find("[data-player='lockmax']").on('click', function () {

                if(!$(this).hasClass('active'))
                    _self._eventLockMax();

            });

            this.$showcase.on('click', '[data-position]', function(){

                _self._eventCenter($(this).data('position'));
                _self._eventScrollToChart();

            });

            //called when key is pressed in textbox
            this.$showcase.find(".quantity").keypress(function (e) {
                //if the letter is not digit then display error and don't type anything
                if ((e.which != 46 || this.value.indexOf(".") != -1) && e.which != 8 && e.which != 0 && (e.which < 48 || e.which > 57)) {
                    //display error message
                    $(".errmsg").html("Digits Only").show().fadeOut("slow");
                    return false;
                }

            });

            this.$showcase.find(".quantity").on('focus', function () {

                if(_self.$showcase.find('[data-player="pause"]').length)
                    _self._eventPause(_self.$showcase.find('[data-player="pause"]'));

            });

//            this.$showcase.find("[data-replayer]").on('click', function () {
//
//                $(this).hide().attr("data-replayer", "no");
//                _self._replay();
//
//            });

            $('#chart' + this.showcaseId).on('click', function(){

                if(_self.$showcase.find('[data-player="pause"]').length)
                    _self._eventPause(_self.$showcase.find('[data-player="pause"]'));

            });

        },
        _eventPlay: function ($target) {

            var _self = this;
            $target.addClass('hide');
            $target.siblings('[data-player="pause"]').removeClass('hide');

//            if($target.siblings('[data-replayer]').attr("data-replayer") == 'no')
//                $target.siblings('[data-replayer]').hide().removeClass('hide').fadeIn().attr("data-replayer", "yes");

            _self._setInterval();

        },
        _eventPause: function($target) {

            var _self = this;

            $target.addClass('hide');
            $target.siblings('[data-player="play"]').removeClass('hide');

            clearInterval(_self.interval);

        },
        _eventForward: function($target) {

            var _self = this;

            var speed = parseInt($target.siblings('[data-player="speed"]').text());

            if(speed >= 1 && speed < 16)
                speed = speed * 2;
            else if(speed == -1)
                speed = speed * -1;
            else if(speed < -1)
                speed = speed / 2;

            $target.siblings('[data-player="speed"]').text(speed);

            if($target.siblings('[data-player="play"]').hasClass('hide'))
            {
                clearInterval(_self.interval);
                _self._eventPlay($target.siblings('[data-player="play"]'));
            }

        },
        _eventBackward: function($target) {

            var _self = this;

            var speed = parseInt($target.siblings('[data-player="speed"]').text());

            if(speed > 1)
                speed = speed / 2;
            else if(speed == 1)
                speed = speed * -1;
            else if(speed <= -1 && speed > -16)
                speed = speed * 2;

            $target.siblings('[data-player="speed"]').text(speed);

            if($target.siblings('[data-player="play"]').hasClass('hide'))
            {
                clearInterval(_self.interval);
                _self._eventPlay($target.siblings('[data-player="play"]'));
            }
        },
        _eventLock: function() {

            var _self = this;
            var $target = _self.$showcase.find("[data-player='lock']");

            $target.addClass('active').siblings().removeClass('active');

            $.each(_self.hc.yAxis, function(index, yaxis) {

                yaxis.setExtremes(
                    yaxis.dataMin,
                    yaxis.dataMax,
                    false
                );

            });
            _self.hc.redraw();
        },
        _eventUnlock: function() {

            var _self = this;
            var $target = _self.$showcase.find("[data-player='unlock']");

            $target.addClass('active').siblings().removeClass('active');

            $.each(_self.hc.yAxis, function(index, yaxis) {

                yaxis.setExtremes(
                    null,
                    null,
                    false
                );

            });
            _self.hc.redraw();
        },
        _eventLockMax: function() {

            var _self = this;
            var $target = _self.$showcase.find("[data-player='lockmax']");

            $target.addClass('active').siblings().removeClass('active');

            $.each(_self.hc.yAxis, function(index, yaxis) {

                if(yaxis.hasVisibleSeries) {
                    yaxis.setExtremes(
                        yaxis.options.globalDataMin,
                        yaxis.options.globalDataMax,
                        false
                    );
                }

            });

            _self.hc.redraw();
        },
        _eventCenter: function(xpos) {

            var radius = (this.hc.xAxis[0].max - this.hc.xAxis[0].min)/2;
            var newmin = xpos - radius;
            var newmax = xpos + radius;

            if(newmin < this.hc.xAxis[0].dataMin) newmin = this.hc.xAxis[0].dataMin;
            if(newmax > this.hc.xAxis[0].dataMax) newmax = this.hc.xAxis[0].dataMin;

            this.hc.xAxis[0].setExtremes(
                newmin,
                newmax
            );

        },
        _eventScrollToChart: function() {

            var _self = this;
            $('html, body').animate({
                scrollTop: _self.$showcase.find('.showcase-player').offset().top - 150
            }, 1000);

        },
        _setInterval: function() {

            var _self = this;
            var frequency = 10;
            var speed = parseInt(_self.$showcase.find('[data-player="speed"]').text());

            clearInterval(_self.interval);
            _self.interval = setInterval(function(){

                if((speed < 0 && _self.min < _self.hc.xAxis[0].min) || (speed > 0 && _self.max > _self.hc.xAxis[0].max)){
                    _self.hc.xAxis[0].setExtremes(
                        _self.hc.xAxis[0].min + ((1 / frequency) * speed),
                        _self.hc.xAxis[0].max + ((1 / frequency) * speed)
                    );
                } else {
                    _self._eventPause(_self.$showcase.find('[data-player="pause"]'))
                }

            }, 1000 / frequency)

        },
        _logSeriesColor: function(name){
            switch (name) {
                case "Events":
                    return "#3498db";
                case "Insults":
                    return "#e74c3c";
                case "Interventions":
                    return "#2ecc71";
            }
        },
        _seriesMax: function(index){

            var max = this.series[index].data[0][1];

            $.each(this.series[index].data, function(index, coordinate) {
                if(coordinate[1] > max)
                    max = coordinate[1];
            });

            return max;

        },
        _seriesMin: function(index){

            var min = this.series[index].data[0][1];

            $.each(this.series[index].data, function(index, coordinate) {
                if(coordinate[1] < min)
                    min = coordinate[1];
            });

            return min;

        },
        _toMMSS: function (second) {

            second = parseInt(second, 10);

            seconds = second % 60;
            if (seconds < 10)
                seconds = "0"+seconds;

            return Math.floor(second/60)+':'+seconds;

        },
        _sortTimeLine: function() {

            this.timeline.sort(function(x, y){

                if (x.time < y.time) {
                    return -1;
                }
                if (x.time > y.time) {
                    return 1;
                }
                return 0;

            });

        },
        _pushToTimeLine: function(type, time, title, content) {

            var bg = 'primary';
            var fa = 'pencil';
            if(type.toLowerCase() == 'events' || type.toLowerCase() == 'actions') {
                bg = 'primary';
                fa = 'tags';
                type = type.substring(0, type.length - 1);
            } else if(type.toLowerCase() == 'insults'){
                bg = 'danger';
                fa = 'exclamation';
                type = type.substring(0, type.length - 1);
            } else if(type.toLowerCase() == 'interventions'){
                bg = 'success';
                fa = 'medkit';
                type = type.substring(0, type.length - 1);
            }

            $entry = $('<div class="timeline-row">' +
                            '<div class="timeline-time">' +
                                '<small>'+type+'</small>' + this._toMMSS(time) + ' min' +
                            '</div>' +
                            '<div class="timeline-icon" data-position="'+time+'">' +
                                '<div class="btn-'+bg+'">' +
                                    '<i class="fa fa-'+fa+'"></i>' +
                                '</div>' +
                            '</div>' +
                            '<div class="panel timeline-content timeline-type-'+bg+'" data-position="'+time+'">' +
                                '<div class="panel-body">' +
                                    '<h2 class="thin">' +
                                    title +
                                    '</h2>' +
                                    '<p>' +
                                    content +
                                    '</p>' +
                                '</div>' +
                            '</div>' +
                        '</div>');

            this.timeline.push({time: parseFloat(time), entry: $entry});

        },
        _pushZoomAlert: function (name) {

            this.$showcase.find('[data-series-alerts]').append('<div class="alert alert-warning" role="alert" data-series-alert="' + name + '">' + name + ' is not available at this zoom level. Data fidelity is too high, please zoom in to visualize it.');

        },
        _removeZoomAlert: function (name) {

            this.$showcase.find('[data-series-alert="'+name+'"]').remove();

        },
        _checkAxisVisibility: function (min, max){

            var _self = this;
            var range = max-min;

            $.each(_self.hc.yAxis, function(index, yaxis) {

                if(yaxis.hasVisibleSeries && yaxis.options.pointsPerSecond * range > _self.pointMax) {

                    yaxis.series[0].hide();
                    yaxis.update({title: {enabled: false}, labels: {enabled: false}});

                    if(!(_self.$showcase.find('[data-series-alert="'+yaxis.series[0].name+'"]').length > 0))
                    {
                      _self._pushZoomAlert(yaxis.series[0].name);
                    }
                } else if(!yaxis.hasVisibleSeries && yaxis.options.pointsPerSecond * range <= _self.pointMax){

                    if(_self.$showcase.find('[data-series-alert="'+yaxis.series[0].name+'"]').length > 0)
                    {
                        _self._removeZoomAlert(yaxis.series[0].name);
                        //yaxis.series[0].show();
                    }

                }

            });

        },
        _addTimeLine: function () {

            var _self = this;

            $.each(this.timeline, function(key, value){

                _self.$showcase.find('[data-timelime]').append(value.entry);

            });

        },
        _addChart: function () {

            var _self = this;

            _self.hc = new Highcharts.Chart({
                chart: {
                    height: 600,
                    type: 'line',
                    zoomType: 'xy',
                    animation: false,
                    width: _self.$showcase.width(),
                    renderTo: 'chart' + _self.showcaseId,
                    events: {
                        selection: function (event) {
                            _self.$showcase.find('[data-series-alert]').remove();
                            if(event.xAxis){
                                var $target = _self.$showcase.find("[data-player='unlock']");
                                _self.$showcase.find("[data-player='lock']").attr('disabled', 'disabled');
                                _self.$showcase.find("[data-player='lockmax']").attr('disabled', 'disabled');
                                $target.addClass('active').siblings().removeClass('active');

                                _self._checkAxisVisibility(this.xAxis[0].min, this.xAxis[0].max);

                                if(_self.origonalZoomMin == -1 && _self.origonalZoomMax == -1){
                                    _self.origonalZoomMin = this.xAxis[0].min;
                                    _self.origonalZoomMax = this.xAxis[0].max;
                                }

                            } else {
                                _self.$showcase.find("[data-player='lock']").removeAttr('disabled');
                                _self.$showcase.find("[data-player='lockmax']").removeAttr('disabled');
                                _self._checkAxisVisibility(_self.origonalZoomMin, _self.origonalZoomMax);
                                _self.origonalZoomMin = -1;
                                _self.origonalZoomMax = -1;
                            }
                        }
                    }
                },
                plotOptions: {
                    series: {
                        events: {
                            legendItemClick: function (event) {

                                if(this.yAxis.options.pointsPerSecond * (this.xAxis.max - this.xAxis.min) > _self.pointMax){
                                    event.preventDefault();
                                    if(!(_self.$showcase.find('[data-series-alert="'+this.name+'"]').length > 0)) {
                                        _self._pushZoomAlert(this.name)
                                    }
                                    return;
                                }

                                if(this.yAxis.hasVisibleSeries)
                                    this.yAxis.update({title: {enabled: false}, labels: {enabled: false}});
                                else
                                    this.yAxis.update({title: {enabled: true}, labels: {enabled: true}});

                                if(_self.$showcase.find("[data-player='lockmax']").hasClass('active'))
                                {
                                    if(!this.yAxis.hasVisibleSeries) {
                                        this.yAxis.setExtremes(
                                            this.yAxis.options.globalDataMin,
                                            this.yAxis.options.globalDataMax
                                        );
                                    }
                                }

                                if(this.visible && _self.$showcase.find("[data-player='lock']").hasClass('active'))
                                {
                                    _self._eventUnlock();
                                }
                            }
                        }
                    }
                },
                yAxis: _self.yaxis,
                xAxis: {
                    min: 0,
                    max: 10,
                    labels: {
                        formatter: function () {
                            return _self._toMMSS(this.value);
                        }
                    },
                    events: {
                        setExtremes: function() {

                            _self._checkAxisVisibility(this.min, this.max);

                        },
                        afterSetExtremes: function() {

                            _self._checkAxisVisibility(this.min, this.max);

                        }
                    }
                },
                navigator: {
                    enabled: true,
                    xAxis: {
                        labels: {
                            formatter: function () {
                                return _self._toMMSS(this.value);
                            }
                        }
                    }
                },
                tooltip: {
                    crosshairs: true,
                    shared: false,
                    headerFormat: ''
                },
                credits: {
                    enabled: false
                },
                rangeSelector: {
                    enabled: false
                },
                scrollbar: {
                    enabled: false
                },
                title: {
                    text: ""
                },
                legend: {
                    enabled: true,
                    layout: 'horizontal',
                    align: 'center',
                    verticalAlign: 'bottom'
                },
                series: _self.series
            });

        },
        _replay: function () {

            var _self = this;

            _self.hc.xAxis[0].setExtremes(
                _self.min,
                _self.min + 60
            );

            _self._setInterval();

        }
    };

    this.logError = function (message) {
        if (this.console) {
            console.error(message);
        }
    };

    $.fn.showcase = function (options) {

        if (typeof options === 'string') {

            var args = Array.prototype.slice.call(arguments, 1);

            this.each(function () {

                var instance = $.data(this, 'showcase');

                if (!instance) {
                    logError("cannot call methods on showcase prior to initialization; " +
                        "attempted to call method '" + options + "'");
                    return;
                }

                if (!$.isFunction(instance[options]) || options.charAt(0) === "_") {
                    logError("no such method '" + options + "' for showcase instance");
                    return;
                }

                instance[options].apply(instance, args);

            });

        }
        else {

            this.each(function () {

                var instance = $.data(this, 'showcase');
                if (!instance) {
                    $.data(this, 'showcase', new $.showcase(options, this));
                }
            });

        }

        return this;

    };

})(jQuery);