/* ========================= GPSD2eCAL LICENSE =================================
 *
 * Copyright (C) 2016 - 2019 Continental Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ========================= GPSD2eCAL LICENSE =================================
*/

#include "gpsdfullmsg.h"
#include "gpshelper.h"
#include "ecal/ecal_time.h"
using namespace utility;

//------------------------------------------------------------------------------
// gpsdfullmsg::gpsdfullmsg
//------------------------------------------------------------------------------
gpsdfullmsg::gpsdfullmsg(Idevice* device, const string &name):gpsmsg(), _gpsd_handler(device)
{
    _pub_gpsdfull.Create(name);
}

//------------------------------------------------------------------------------
// gpsdfullmsg::set_gps_message()
//------------------------------------------------------------------------------
void gpsdfullmsg::set_gps_message()
{
    set_gps_data();
    set_gps_fix_data();
    set_gps_gst();
    set_gps_rtcm2();
    set_gps_rtcm3_t();
    set_gps_subframe();
    set_gps_navdata();
    set_gps_attitude();
    set_gps_devconfig();
    set_gps_dop();
}

//------------------------------------------------------------------------------
// gpsdfullmsg::send_gps_message()
//------------------------------------------------------------------------------
void gpsdfullmsg::send_gps_message()
{
    // read the new gps data
    _gps_data = update(_gpsd_handler);

    // set all gps data to proto struct
    set_gps_message();

    // publish message to ecal
    _pub_gpsdfull.Send(_msg_gpsdfull);
}

//------------------------------------------------------------------------------
// gpsdfullmsg::set_gps_data()
//------------------------------------------------------------------------------
void gpsdfullmsg::set_gps_data()
{
    _msg_gpsdfull.set_satellites_used   (_gps_data.satellites_used);
    _msg_gpsdfull.set_satellites_visible(_gps_data.satellites_visible);
#if GPSD_API_MAJOR_VERSION < 9
    _msg_gpsdfull.set_separation        (_gps_data.separation);
    _msg_gpsdfull.set_epe               (_gps_data.epe);
    _msg_gpsdfull.set_status            (_gps_data.status);
    _msg_gpsdfull.set_online            (_gps_data.online);
#endif
}

//------------------------------------------------------------------------------
// gpsdfullmsg::set_gps_fix_data()
//------------------------------------------------------------------------------
void gpsdfullmsg::set_gps_fix_data()
{
    // Set the timestamp
    auto header = new pb::Header();
    header->set_timestamp             (eCAL::Time::GetMicroSeconds());
    _msg_gpsdfull.set_allocated_header(move(header));

    auto fix_data = new pb::gps::GPSDFull_gps_fix_data();
    if(nullptr == fix_data) return;

    fix_data->set_altitude              (_gps_data.fix.altitude);
    fix_data->set_climb                 (_gps_data.fix.climb);
    fix_data->set_latitude              (_gps_data.fix.latitude);
    fix_data->set_epc                   (_gps_data.fix.epc);
    fix_data->set_epd                   (_gps_data.fix.epd);
    fix_data->set_eps                   (_gps_data.fix.eps);
    fix_data->set_ept                   (_gps_data.fix.ept);
    fix_data->set_epv                   (_gps_data.fix.epv);
    fix_data->set_epx                   (_gps_data.fix.epx);
    fix_data->set_epy                   (_gps_data.fix.epy);
    fix_data->set_longitude             (_gps_data.fix.longitude);
    fix_data->set_mode                  (_gps_data.fix.mode);
    fix_data->set_speed                 (_gps_data.fix.speed);
    fix_data->set_track                 (_gps_data.fix.track);
    _msg_gpsdfull.set_allocated_fix_data(move(fix_data));
}

//------------------------------------------------------------------------------
// gpsdfullmsg::set_gps_gst()
//------------------------------------------------------------------------------
void gpsdfullmsg::set_gps_gst()
{
    /*
    * The structure describing the pseudorange errors (GPGST)
    */
    auto gst = new pb::gps::GPSDFull::gps_gst_t();
    gst->set_alt_err_deviation     (_gps_data.gst.alt_err_deviation);
    gst->set_lat_err_deviation     (_gps_data.gst.lat_err_deviation);
    gst->set_lon_err_deviation     (_gps_data.gst.lon_err_deviation);
    gst->set_rms_deviation         (_gps_data.gst.rms_deviation);
    gst->set_smajor_deviation      (_gps_data.gst.smajor_deviation);
    gst->set_smajor_orientation    (_gps_data.gst.smajor_orientation);
    gst->set_sminor_deviation      (_gps_data.gst.sminor_deviation);
#if GPSD_API_MAJOR_VERSION < 9
    gst->set_utctime               (_gps_data.gst.utctime);
#endif
    _msg_gpsdfull.set_allocated_gst_t(move(gst));
}

//------------------------------------------------------------------------------
// gpsdfullmsg::set_gps_rtcm2()
//------------------------------------------------------------------------------
void gpsdfullmsg::set_gps_rtcm2()
{
    auto rtcm2 = new pb::gps::GPSDFull_rtcm2_t();
    rtcm2->set_refstaid   (_gps_data.rtcm2.refstaid);
    rtcm2->set_seqnum     (_gps_data.rtcm2.seqnum);
    rtcm2->set_stathlth   (_gps_data.rtcm2.stathlth);
    rtcm2->set_type       (_gps_data.rtcm2.type);
    rtcm2->set_zcount     (_gps_data.rtcm2.zcount);

    auto gps_almanac = new pb::gps::GPSDFull_rtcm2_t_gps_almanac();
    gps_almanac->set_nentries(_gps_data.rtcm2.almanac.nentries);
    rtcm2->set_allocated_almanac(move(gps_almanac));

    auto conhealt = new pb::gps::GPSDFull_rtcm2_t_gps_conhealth();
    conhealt->set_nentries(_gps_data.rtcm2.conhealth.nentries);
    rtcm2->set_allocated_conhealth(move(conhealt));

    auto ecef = new pb::gps::GPSDFull_rtcm2_t_gps_ecef();
    ecef->set_valid(_gps_data.rtcm2.ecef.valid);
    ecef->set_x    (_gps_data.rtcm2.ecef.x);
    ecef->set_y    (_gps_data.rtcm2.ecef.y);
    ecef->set_z    (_gps_data.rtcm2.ecef.z);
    rtcm2->set_allocated_ecef(move(ecef));

    auto golnass_ranges = new pb::gps::GPSDFull_rtcm2_t_gps_glonass_ranges();
    golnass_ranges->set_nentries       (_gps_data.rtcm2.glonass_ranges.nentries);
    rtcm2->set_allocated_glonass_ranges(move(golnass_ranges));

    auto gpstime = new pb::gps::GPSDFull_rtcm2_t_gps_time();
    gpstime->set_hour           (_gps_data.rtcm2.gpstime.hour);
    gpstime->set_leapsecs       (_gps_data.rtcm2.gpstime.leapsecs);
    gpstime->set_week           (_gps_data.rtcm2.gpstime.week);
    rtcm2->set_allocated_gpstime(move(gpstime));

    auto ranges = new pb::gps::GPSDFull_rtcm2_t_gps_ranges();
    ranges->set_nentries(_gps_data.rtcm2.gps_ranges.nentries);
    rtcm2->set_allocated_ranges(move(ranges));

    auto reference = new pb::gps::GPSDFull_rtcm2_t_gps_reference();
    reference->set_valid    (_gps_data.rtcm2.reference.valid);
    reference->set_system   (_gps_data.rtcm2.reference.system);
    reference->set_sense    (_gps_data.rtcm2.reference.sense);
    reference->set_dx       (_gps_data.rtcm2.reference.dx);
    reference->set_dy       (_gps_data.rtcm2.reference.dy);
    reference->set_dz       (_gps_data.rtcm2.reference.dz);
    rtcm2->set_allocated_reference(move(reference));

    auto xmitter = new pb::gps::GPSDFull_rtcm2_t_gps_xmitter();
    xmitter->set_lat        (_gps_data.rtcm2.xmitter.lat);
    xmitter->set_lon        (_gps_data.rtcm2.xmitter.lon);
    xmitter->set_range      (_gps_data.rtcm2.xmitter.range);
    xmitter->set_rangeflag  (_gps_data.rtcm2.xmitter.rangeflag);
    xmitter->set_status     (_gps_data.rtcm2.xmitter.status);
    rtcm2->set_allocated_xmitter(move(xmitter));

    _msg_gpsdfull.set_allocated_rtcm2(move(rtcm2));
}

//------------------------------------------------------------------------------
// gpsdfullmsg::set_gps_rtcm3_t()
//------------------------------------------------------------------------------
void gpsdfullmsg::set_gps_rtcm3_t()
{
    auto rtcm3 = new pb::gps::GPSDFull_rtcm3_t();
    rtcm3->set_length(_gps_data.rtcm3.length);

    auto rtcm3_01 = new pb::gps::GPSDFull_rtcm3_t_rtcm3_1001();
    auto header_01 =  rtcm3_01->mutable_header();
    if(nullptr == header_01) return;
    header_01->set_interval     (_gps_data.rtcm3.rtcmtypes.rtcm3_1001.header.interval);
    header_01->set_satcount     (_gps_data.rtcm3.rtcmtypes.rtcm3_1001.header.satcount);
    header_01->set_smoothing    (_gps_data.rtcm3.rtcmtypes.rtcm3_1001.header.smoothing);
    header_01->set_station_id   (_gps_data.rtcm3.rtcmtypes.rtcm3_1001.header.station_id);
    header_01->set_sync         (_gps_data.rtcm3.rtcmtypes.rtcm3_1001.header.sync);
    rtcm3->set_allocated_rtcm3_01(move(rtcm3_01));

    auto rtcm3_02 = new pb::gps::GPSDFull_rtcm3_t_rtcm3_1002();
    auto header_02 =  rtcm3_02->mutable_header();
    if(nullptr == header_02) return;
    header_02->set_interval     (_gps_data.rtcm3.rtcmtypes.rtcm3_1002.header.interval);
    header_02->set_satcount     (_gps_data.rtcm3.rtcmtypes.rtcm3_1002.header.satcount);
    header_02->set_smoothing    (_gps_data.rtcm3.rtcmtypes.rtcm3_1002.header.smoothing);
    header_02->set_station_id   (_gps_data.rtcm3.rtcmtypes.rtcm3_1002.header.station_id);
    header_02->set_sync         (_gps_data.rtcm3.rtcmtypes.rtcm3_1002.header.sync);
    rtcm3->set_allocated_rtcm3_02(move(rtcm3_02));

    auto rtcm3_03 = new pb::gps::GPSDFull_rtcm3_t_rtcm3_1003();
    auto header_03 =  rtcm3_03->mutable_header();
    if(nullptr == header_03) return;
    header_03->set_interval     (_gps_data.rtcm3.rtcmtypes.rtcm3_1003.header.interval);
    header_03->set_satcount     (_gps_data.rtcm3.rtcmtypes.rtcm3_1003.header.satcount);
    header_03->set_smoothing    (_gps_data.rtcm3.rtcmtypes.rtcm3_1003.header.smoothing);
    header_03->set_station_id   (_gps_data.rtcm3.rtcmtypes.rtcm3_1003.header.station_id);
    header_03->set_sync         (_gps_data.rtcm3.rtcmtypes.rtcm3_1003.header.sync);
    rtcm3->set_allocated_rtcm3_03(move(rtcm3_03));

    auto rtcm3_04 = new pb::gps::GPSDFull_rtcm3_t_rtcm3_1004();
    auto header_04 =  rtcm3_04->mutable_header();
    if(nullptr == header_04) return;
    header_04->set_interval     (_gps_data.rtcm3.rtcmtypes.rtcm3_1004.header.interval);
    header_04->set_satcount     (_gps_data.rtcm3.rtcmtypes.rtcm3_1004.header.satcount);
    header_04->set_smoothing    (_gps_data.rtcm3.rtcmtypes.rtcm3_1004.header.smoothing);
    header_04->set_station_id   (_gps_data.rtcm3.rtcmtypes.rtcm3_1004.header.station_id);
    header_04->set_sync         (_gps_data.rtcm3.rtcmtypes.rtcm3_1004.header.sync);
    rtcm3->set_allocated_rtcm3_04(move(rtcm3_04));

    _msg_gpsdfull.set_allocated_rtcm3(move(rtcm3));
}

//------------------------------------------------------------------------------
// gpsdfullmsg::set_gps_subframe()
//------------------------------------------------------------------------------
void gpsdfullmsg::set_gps_subframe()
{
    auto subframe = new pb::gps::GPSDFull_subframe_t();
    subframe->set_alert         (_gps_data.subframe.alert);
    subframe->set_antispoof     (_gps_data.subframe.antispoof);
    subframe->set_data_id       (_gps_data.subframe.data_id);
    subframe->set_integrity     (_gps_data.subframe.integrity);
    subframe->set_is_almanac    (_gps_data.subframe.is_almanac);
    subframe->set_l_tow17       (_gps_data.subframe.l_TOW17);
    subframe->set_pageid        (_gps_data.subframe.pageid);
    subframe->set_subframe_num  (_gps_data.subframe.subframe_num);
    subframe->set_tow17         (_gps_data.subframe.TOW17);
    subframe->set_tsvid         (_gps_data.subframe.tSVID);

    //sub1
    auto subframe1 = new pb::gps::GPSDFull_subframe_t_sub1();
    subframe1->set_af0     (_gps_data.subframe.sub1.af0);
    subframe1->set_af1     (_gps_data.subframe.sub1.af1);
    subframe1->set_af2     (_gps_data.subframe.sub1.af2);
    subframe1->set_d_af0   (_gps_data.subframe.sub1.d_af0);
    subframe1->set_d_af1   (_gps_data.subframe.sub1.d_af1);
    subframe1->set_d_af2   (_gps_data.subframe.sub1.d_af2);
    subframe1->set_d_tgd   (_gps_data.subframe.sub1.d_Tgd);
    subframe1->set_hlth    (_gps_data.subframe.sub1.hlth);
    subframe1->set_iodc    (_gps_data.subframe.sub1.IODC);
    subframe1->set_l2      (_gps_data.subframe.sub1.l2);
    subframe1->set_l2p     (_gps_data.subframe.sub1.l2p);
    subframe1->set_l_toc   (_gps_data.subframe.sub1.l_toc);
    subframe1->set_tgd     (_gps_data.subframe.sub1.Tgd);
    subframe1->set_toc     (_gps_data.subframe.sub1.toc);
    subframe1->set_ura     (_gps_data.subframe.sub1.ura);
    subframe1->set_wn      (_gps_data.subframe.sub1.WN);
    subframe->set_allocated_subframe1(move(subframe1));

    //sub2
    auto subframe2 = new pb::gps::GPSDFull_subframe_t_sub2();
    subframe2->set_aodo                (_gps_data.subframe.sub2.AODO);
    subframe2->set_crs                 (_gps_data.subframe.sub2.Crs);
    subframe2->set_cuc                 (_gps_data.subframe.sub2.Cuc);
    subframe2->set_cus                 (_gps_data.subframe.sub2.Cus);
    subframe2->set_deltan              (_gps_data.subframe.sub2.deltan);
    subframe2->set_d_crs               (_gps_data.subframe.sub2.d_Crs);
    subframe2->set_d_cuc               (_gps_data.subframe.sub2.d_Cuc);
    subframe2->set_cus                 (_gps_data.subframe.sub2.d_Cus);
    subframe2->set_d_deltan            (_gps_data.subframe.sub2.d_deltan);
    subframe2->set_d_eccentricity      (_gps_data.subframe.sub2.d_eccentricity);
    subframe2->set_d_m0                (_gps_data.subframe.sub2.d_M0);
    subframe2->set_d_sqrta             (_gps_data.subframe.sub2.d_sqrtA);
    subframe2->set_e                   (_gps_data.subframe.sub2.e);
    subframe2->set_fit                 (_gps_data.subframe.sub2.fit);
    subframe2->set_iode                (_gps_data.subframe.sub2.IODE);
    subframe2->set_l_toe               (_gps_data.subframe.sub2.l_toe);
    subframe2->set_m0                  (_gps_data.subframe.sub2.M0);
    subframe2->set_sqrta               (_gps_data.subframe.sub2.sqrtA);
    subframe2->set_toe                 (_gps_data.subframe.sub2.toe);
    subframe2->set_u_aodo              (_gps_data.subframe.sub2.u_AODO);
    subframe->set_allocated_subframe2(move(subframe2));

    _msg_gpsdfull.set_allocated_subframe(move(subframe));
}
//------------------------------------------------------------------------------
// gpsdfullmsg::set_gps_navdata()
//------------------------------------------------------------------------------
void gpsdfullmsg::set_gps_navdata()
{
    auto navdata = new pb::gps::GPSDFull_navdata_t();
    navdata->set_air_pressure       (_gps_data.navdata.air_pressure);
    navdata->set_air_temp           (_gps_data.navdata.air_temp);
    navdata->set_compass_deviation  (_gps_data.navdata.compass_deviation);
    navdata->set_compass_heading    (_gps_data.navdata.compass_heading);
    navdata->set_compass_status     (_gps_data.navdata.compass_status);
    navdata->set_compass_variation  (_gps_data.navdata.compass_variation);
    navdata->set_crosstrack_error   (_gps_data.navdata.crosstrack_error);
    navdata->set_crosstrack_status  (_gps_data.navdata.crosstrack_status);
    navdata->set_depth              (_gps_data.navdata.depth);
    navdata->set_depth_offset       (_gps_data.navdata.depth_offset);
    navdata->set_log_cumulative     (_gps_data.navdata.log_cumulative);
    navdata->set_log_trip           (_gps_data.navdata.log_trip);
    navdata->set_version            (_gps_data.navdata.version);
    navdata->set_water_temp         (_gps_data.navdata.water_temp);
    navdata->set_wind_dir           (_gps_data.navdata.wind_dir);
    navdata->set_wind_speed         (_gps_data.navdata.wind_speed);
    _msg_gpsdfull.set_allocated_navdata(move(navdata));
}

//------------------------------------------------------------------------------
// gpsdfullmsg::set_gps_attitude()
//------------------------------------------------------------------------------
void gpsdfullmsg::set_gps_attitude()
{
    auto attitude = new pb::gps::GPSDFull_attitude_t();
    attitude->set_acc_len           (_gps_data.attitude.acc_len);
    attitude->set_acc_x             (_gps_data.attitude.acc_x);
    attitude->set_acc_y             (_gps_data.attitude.acc_y);
    attitude->set_acc_z             (_gps_data.attitude.acc_z);
    attitude->set_depth             (_gps_data.attitude.depth);
    attitude->set_dip               (_gps_data.attitude.dip);
    attitude->set_gyro_x            (_gps_data.attitude.gyro_x);
    attitude->set_gyro_y            (_gps_data.attitude.gyro_y);
    attitude->set_heading           (_gps_data.attitude.heading);
    attitude->set_mag_len           (_gps_data.attitude.mag_len);
    attitude->set_mag_st            (_gps_data.attitude.mag_st);
    attitude->set_mag_x             (_gps_data.attitude.mag_x);
    attitude->set_mag_y             (_gps_data.attitude.mag_y);
    attitude->set_mag_z             (_gps_data.attitude.mag_z);
    attitude->set_pitch             (_gps_data.attitude.pitch);
    attitude->set_pitch_st          (_gps_data.attitude.pitch_st);
    attitude->set_roll              (_gps_data.attitude.roll);
    attitude->set_roll_st           (_gps_data.attitude.roll_st);
    attitude->set_temp              (_gps_data.attitude.temp);
    attitude->set_yaw               (_gps_data.attitude.yaw);
    attitude->set_yaw_st            (_gps_data.attitude.yaw_st);
    _msg_gpsdfull.set_allocated_attitude(move(attitude));
}

//------------------------------------------------------------------------------
// gpsdfullmsg::set_gps_devconfig()
//------------------------------------------------------------------------------
void gpsdfullmsg::set_gps_devconfig()
{
    string parity ;
    parity = _gps_data.dev.parity;
    auto devconfig = new pb::gps::GPSDFull_devconfig_t();
    devconfig->set_baudrate         (_gps_data.dev.baudrate);
    devconfig->set_driver_mode      (_gps_data.dev.driver_mode);
    devconfig->set_flags            (_gps_data.dev.flags);
    devconfig->set_stopbits         (_gps_data.dev.stopbits);
#if GPSD_API_MAJOR_VERSION < 9
    devconfig->set_activated        (_gps_data.dev.activated);
    devconfig->set_mincycle         (_gps_data.dev.mincycle);
    devconfig->set_cycle            (_gps_data.dev.cycle);
#endif

    devconfig->set_parity           (isUtf8(parity.c_str())        ? parity.c_str()        : _gpsd_undefined);
    devconfig->set_path             (isUtf8(_gps_data.dev.path)    ? _gps_data.dev.path    : _gpsd_undefined);
    devconfig->set_subtype          (isUtf8(_gps_data.dev.subtype) ? _gps_data.dev.subtype : _gpsd_undefined);
    devconfig->set_driver           (isUtf8(_gps_data.dev.driver)  ? _gps_data.dev.driver  : _gpsd_undefined);

    _msg_gpsdfull.set_allocated_devconfig(move(devconfig));
}

//------------------------------------------------------------------------------
// gpsdfullmsg::set_gps_dop()
//------------------------------------------------------------------------------
void gpsdfullmsg::set_gps_dop()
{
    /* Dilution of precision factors */
    auto dop = new pb::gps::GPSDFull_dop_t();
    dop->set_gdop       (_gps_data.dop.gdop);
    dop->set_hdop       (_gps_data.dop.hdop);
    dop->set_pdop       (_gps_data.dop.pdop);
    dop->set_tdop       (_gps_data.dop.tdop);
    dop->set_vdop       (_gps_data.dop.vdop);
    dop->set_xdop       (_gps_data.dop.xdop);
    dop->set_ydop       (_gps_data.dop.ydop);
    _msg_gpsdfull.set_allocated_dop(move(dop));
}
