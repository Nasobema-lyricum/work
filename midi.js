//  ==============================================================
/**
    @file   views.js
    @brief  DD-500_Editor controller
            
            MVCのcontroller。
            backboneではviewと呼ばれる。

    @author kurosawa
    @date   2016/08/18

    Copyright (C) 2016 Roland Corporation.
 */
//  ==============================================================
dbg.log('load views.js');
// --------------------------------------------------------------
/**
    @brief      value spinner view

    @author     kurosawa
    @date       2016/08/18
    @return     void
  */
// --------------------------------------------------------------
var ValueSpinnerView = Backbone.View.extend({
    events: {
        'spinstop': 'stop',
        'spin':     'change'
    },
    initialize: function() {

        this.$el.spinner({
            max: this.attributes.max,
            min: this.attributes.min
        });
    },
    render: function(val) {
        // dbg.log('spinner render ' + this.$el.attr('id'));

        if ( val == 0 ) { this.renderOFF();               }
        else            { this.$el.spinner('value', val); }
        return this;
    },
    change: function(event, ui) {
        // dbg.log('spinner change ' + this.$el.attr('id'));
        this.trigger('change', ui.value);

        if (ui.value == 0) this.renderOFF();
    },
    stop: function(event, ui) {
        // dbg.log('spinner stop ' + this.$el.attr('id'));
        var value = this.$el.spinner('value');
        this.trigger('stop', value);

        if (value == 0) { this.renderOFF(); }
    },
    get: function() {
        this.$el.spnner('value');
    },
    enable: function() {
        // dbg.log('spinner enable ' + this.$el.attr('id'));
        this.$el.spinner('enable');
    },
    disable: function() {
        // dbg.log('spinner disable ' + this.$el.attr('id'));
        this.$el.spinner('disable');
    },
    renderOFF: function() { this.$el.spinner('value', 'OFF'); }
});

// --------------------------------------------------------------
/**
  @brief      value slider view

  @author     kurosawa
  @date       2016/08/18
  @return     void
  */
// --------------------------------------------------------------
var ValueSliderView = Backbone.View.extend({
    events: {
        'slidestop': 'stop',
        'slide':     'change'
    },

    initialize: function() {

        this.$el.slider({
            max: this.attributes.max,
            min: this.attributes.min
        });
    },
    render: function(val) {
        // dbg.log('slider render ' + this.$el.attr('id'));
        this.$el.slider('value', val);
        return this;
    },
    change: function(event, ui) {
        // dbg.log('slider change ' + this.$el.attr('id'));
        this.trigger('change', ui.value);
    },
    stop: function(event, ui) {
        // dbg.log('slider stop ' + this.$el.attr('id'));
        this.trigger('stop', ui.value);
    },
    get: function() {
        this.$el.slider('value');
    },
    enable: function() {
        // dbg.log('slider enable ' + this.$el.attr('id'));
        this.$el.slider('enable');
    },
    disable: function() {
        // dbg.log('slider disable ' + this.$el.attr('id'));
        this.$el.slider('disable');
    }
});

// --------------------------------------------------------------
/**
  @brief      sw view

  @author     kurosawa
  @date       2016/08/18
  @return     void
  */
// --------------------------------------------------------------
var SwView = Backbone.View.extend({
    events: {
        'click input': 'change'
    },

    initialize: function() {
        // dbg.log('sw initialize' + this.$el.attr('id'));
        this.model.bind('change',  _.bind(this.render, this));
        this.model.bind('invalid', _.bind(this.render, this));
    },
    render: function() {
        // dbg.log('sw render ' + this.$el.attr('id'));
        var value = this.model.get('value');
        if (value == 0) { this.$( '#swOff' ).attr('checked', true); }
        else            { this.$( '#swOn'  ).attr('checked', true); }
        this.trigger('enable', value);
        return this;
    },
    change: function(event, ui) {
        // dbg.log('sw change ' + this.$el.attr('id'));
        var value = this.$('input:checked').val();
        if( this.model.set('value', value, {validate: true}) ) { this.model.sendMIDI(); }
    }
});

// --------------------------------------------------------------
/**
  @brief      spinnerSlider view

  @author     kurosawa
  @date       2016/08/25
  @return     void
  */
// --------------------------------------------------------------
var SpinnerSliderView = Backbone.View.extend({
    initialize: function() {
        var max = this.model.get('max');
        var min = this.model.get('min');

        this.spinnerView  = new ValueSpinnerView( {el: this.$( '#valueSpinner' ), attributes: {max: max, min: min}} );
        this.sliderView   = new ValueSliderView(  {el: this.$( '#valueSlider'  ), attributes: {max: max, min: min}} );

        this.model.bind('change',  _.bind(this.render, this));
        this.model.bind('invalid', _.bind(this.render, this));

        this.listenTo( this.spinnerView, 'change', this.changeSpinner );
        this.listenTo( this.spinnerView, 'stop',   this.stop          );

        this.listenTo( this.sliderView,  'change', this.changeSlider  );
        this.listenTo( this.sliderView,  'stop',   this.stop          );

        this.render();
    },
    changeSpinner: function(value) {
        // dbg.log('spinnerSlider changeSpinner ' + this.$el.attr('id'));
        this.sliderView.render(value);
    },
    changeSlider: function(value) {
        // dbg.log('spinnerSlider changeSlider ' + this.$el.attr('id'));
        this.spinnerView.render(value);
    },
    stop: function(value) {
        // dbg.log('spinnerSlider stop ' + this.$el.attr('id'));
        if( this.model.set('value', value, {validate: true}) ) { this.model.sendMIDI(); }
    },
    render: function() {
        // dbg.log('spinnerSlider render ' + this.$el.attr('id'));
        var value = this.model.get('value');
        this.spinnerView.render(value);
        this.sliderView.render(value);
        return this;
    },
    enable: function(value) {
        if (value == 0) {
            this.spinnerView.disable();
            this.sliderView.disable();
        }
        else {
            this.spinnerView.enable();
            this.sliderView.enable();
        }
    }
});

// --------------------------------------------------------------
/**
  @brief      container view
              
              SW, spinner, sliderを含んだview

  @author     kurosawa
  @date       2016/08/18
  @return     void
  */
// --------------------------------------------------------------
var ContainerView = Backbone.View.extend({
    initialize: function() {
        this.swView             = new SwView(            { el: this.$('#swRadio'),       model: this.attributes.swModel    } );
        this.spinnerSliderView  = new SpinnerSliderView( { el: this.$('#spinnerSlider'), model: this.attributes.valueModel } );

        this.listenTo(this.swView, 'enable', this.enable);

        this.spinnerSliderView.render();
        this.swView.render();
    },
    enable: function(value) { this.spinnerSliderView.enable(value); }
});

dbg.log('loaded views.js');
/* vim: set ts=8 sw=4 sts=4 expandtab: */
