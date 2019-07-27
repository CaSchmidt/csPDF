### Project ##################################################################

set(fdrm_SRCS
  core/fdrm/crypto/fx_crypt.cpp
  core/fdrm/crypto/fx_crypt_aes.cpp
  core/fdrm/crypto/fx_crypt_sha.cpp
  )

set(formfiller_SRCS
  fpdfsdk/formfiller/cba_fontmap.cpp
  fpdfsdk/formfiller/cffl_checkbox.cpp
  fpdfsdk/formfiller/cffl_combobox.cpp
  fpdfsdk/formfiller/cffl_formfiller.cpp
  fpdfsdk/formfiller/cffl_iformfiller.cpp
  fpdfsdk/formfiller/cffl_listbox.cpp
  fpdfsdk/formfiller/cffl_pushbutton.cpp
  fpdfsdk/formfiller/cffl_radiobutton.cpp
  fpdfsdk/formfiller/cffl_textfield.cpp
  )

set(fpdfapi_SRCS
  core/fpdfapi/cpdf_modulemgr.cpp
  core/fpdfapi/fpdf_cmaps/CNS1/Adobe-CNS1-UCS2_5.cpp
  core/fpdfapi/fpdf_cmaps/CNS1/B5pc-H_0.cpp
  core/fpdfapi/fpdf_cmaps/CNS1/B5pc-V_0.cpp
  core/fpdfapi/fpdf_cmaps/CNS1/CNS-EUC-H_0.cpp
  core/fpdfapi/fpdf_cmaps/CNS1/CNS-EUC-V_0.cpp
  core/fpdfapi/fpdf_cmaps/CNS1/ETen-B5-H_0.cpp
  core/fpdfapi/fpdf_cmaps/CNS1/ETen-B5-V_0.cpp
  core/fpdfapi/fpdf_cmaps/CNS1/ETenms-B5-H_0.cpp
  core/fpdfapi/fpdf_cmaps/CNS1/ETenms-B5-V_0.cpp
  core/fpdfapi/fpdf_cmaps/CNS1/HKscs-B5-H_5.cpp
  core/fpdfapi/fpdf_cmaps/CNS1/HKscs-B5-V_5.cpp
  core/fpdfapi/fpdf_cmaps/CNS1/UniCNS-UCS2-H_3.cpp
  core/fpdfapi/fpdf_cmaps/CNS1/UniCNS-UCS2-V_3.cpp
  core/fpdfapi/fpdf_cmaps/CNS1/UniCNS-UTF16-H_0.cpp
  core/fpdfapi/fpdf_cmaps/CNS1/cmaps_cns1.cpp
  core/fpdfapi/fpdf_cmaps/GB1/Adobe-GB1-UCS2_5.cpp
  core/fpdfapi/fpdf_cmaps/GB1/GB-EUC-H_0.cpp
  core/fpdfapi/fpdf_cmaps/GB1/GB-EUC-V_0.cpp
  core/fpdfapi/fpdf_cmaps/GB1/GBK-EUC-H_2.cpp
  core/fpdfapi/fpdf_cmaps/GB1/GBK-EUC-V_2.cpp
  core/fpdfapi/fpdf_cmaps/GB1/GBK2K-H_5.cpp
  core/fpdfapi/fpdf_cmaps/GB1/GBK2K-V_5.cpp
  core/fpdfapi/fpdf_cmaps/GB1/GBKp-EUC-H_2.cpp
  core/fpdfapi/fpdf_cmaps/GB1/GBKp-EUC-V_2.cpp
  core/fpdfapi/fpdf_cmaps/GB1/GBpc-EUC-H_0.cpp
  core/fpdfapi/fpdf_cmaps/GB1/GBpc-EUC-V_0.cpp
  core/fpdfapi/fpdf_cmaps/GB1/UniGB-UCS2-H_4.cpp
  core/fpdfapi/fpdf_cmaps/GB1/UniGB-UCS2-V_4.cpp
  core/fpdfapi/fpdf_cmaps/GB1/cmaps_gb1.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/83pv-RKSJ-H_1.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/90ms-RKSJ-H_2.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/90ms-RKSJ-V_2.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/90msp-RKSJ-H_2.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/90msp-RKSJ-V_2.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/90pv-RKSJ-H_1.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/Add-RKSJ-H_1.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/Add-RKSJ-V_1.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/Adobe-Japan1-UCS2_4.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/EUC-H_1.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/EUC-V_1.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/Ext-RKSJ-H_2.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/Ext-RKSJ-V_2.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/H_1.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/UniJIS-UCS2-HW-H_4.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/UniJIS-UCS2-HW-V_4.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/UniJIS-UCS2-H_4.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/UniJIS-UCS2-V_4.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/V_1.cpp
  core/fpdfapi/fpdf_cmaps/Japan1/cmaps_japan1.cpp
  core/fpdfapi/fpdf_cmaps/Korea1/Adobe-Korea1-UCS2_2.cpp
  core/fpdfapi/fpdf_cmaps/Korea1/KSC-EUC-H_0.cpp
  core/fpdfapi/fpdf_cmaps/Korea1/KSC-EUC-V_0.cpp
  core/fpdfapi/fpdf_cmaps/Korea1/KSCms-UHC-HW-H_1.cpp
  core/fpdfapi/fpdf_cmaps/Korea1/KSCms-UHC-HW-V_1.cpp
  core/fpdfapi/fpdf_cmaps/Korea1/KSCms-UHC-H_1.cpp
  core/fpdfapi/fpdf_cmaps/Korea1/KSCms-UHC-V_1.cpp
  core/fpdfapi/fpdf_cmaps/Korea1/KSCpc-EUC-H_0.cpp
  core/fpdfapi/fpdf_cmaps/Korea1/UniKS-UCS2-H_1.cpp
  core/fpdfapi/fpdf_cmaps/Korea1/UniKS-UCS2-V_1.cpp
  core/fpdfapi/fpdf_cmaps/Korea1/UniKS-UTF16-H_0.cpp
  core/fpdfapi/fpdf_cmaps/Korea1/cmaps_korea1.cpp
  core/fpdfapi/fpdf_cmaps/fpdf_cmaps.cpp
  core/fpdfapi/fpdf_edit/cpdf_pagecontentgenerator.cpp
  core/fpdfapi/fpdf_edit/fpdf_edit_create.cpp
  core/fpdfapi/fpdf_font/cpdf_cidfont.cpp
  core/fpdfapi/fpdf_font/cpdf_font.cpp
  core/fpdfapi/fpdf_font/cpdf_fontencoding.cpp
  core/fpdfapi/fpdf_font/cpdf_simplefont.cpp
  core/fpdfapi/fpdf_font/cpdf_truetypefont.cpp
  core/fpdfapi/fpdf_font/cpdf_type1font.cpp
  core/fpdfapi/fpdf_font/cpdf_type3char.cpp
  core/fpdfapi/fpdf_font/cpdf_type3font.cpp
  core/fpdfapi/fpdf_font/fpdf_font.cpp
  core/fpdfapi/fpdf_font/fpdf_font_cid.cpp
  core/fpdfapi/fpdf_font/ttgsubtable.cpp
  core/fpdfapi/fpdf_page/cpdf_allstates.cpp
  core/fpdfapi/fpdf_page/cpdf_clippath.cpp
  core/fpdfapi/fpdf_page/cpdf_clippathdata.cpp
  core/fpdfapi/fpdf_page/cpdf_color.cpp
  core/fpdfapi/fpdf_page/cpdf_colorspace.cpp
  core/fpdfapi/fpdf_page/cpdf_colorstate.cpp
  core/fpdfapi/fpdf_page/cpdf_colorstatedata.cpp
  core/fpdfapi/fpdf_page/cpdf_contentmark.cpp
  core/fpdfapi/fpdf_page/cpdf_contentmarkdata.cpp
  core/fpdfapi/fpdf_page/cpdf_contentmarkitem.cpp
  core/fpdfapi/fpdf_page/cpdf_form.cpp
  core/fpdfapi/fpdf_page/cpdf_formobject.cpp
  core/fpdfapi/fpdf_page/cpdf_generalstate.cpp
  core/fpdfapi/fpdf_page/cpdf_generalstatedata.cpp
  core/fpdfapi/fpdf_page/cpdf_graphicstates.cpp
  core/fpdfapi/fpdf_page/cpdf_image.cpp
  core/fpdfapi/fpdf_page/cpdf_imageobject.cpp
  core/fpdfapi/fpdf_page/cpdf_meshstream.cpp
  core/fpdfapi/fpdf_page/cpdf_page.cpp
  core/fpdfapi/fpdf_page/cpdf_pagemodule.cpp
  core/fpdfapi/fpdf_page/cpdf_pageobject.cpp
  core/fpdfapi/fpdf_page/cpdf_pageobjectholder.cpp
  core/fpdfapi/fpdf_page/cpdf_pageobjectlist.cpp
  core/fpdfapi/fpdf_page/cpdf_pathobject.cpp
  core/fpdfapi/fpdf_page/cpdf_pattern.cpp
  core/fpdfapi/fpdf_page/cpdf_shadingobject.cpp
  core/fpdfapi/fpdf_page/cpdf_shadingpattern.cpp
  core/fpdfapi/fpdf_page/cpdf_textobject.cpp
  core/fpdfapi/fpdf_page/cpdf_textstate.cpp
  core/fpdfapi/fpdf_page/cpdf_textstatedata.cpp
  core/fpdfapi/fpdf_page/cpdf_tilingpattern.cpp
  core/fpdfapi/fpdf_page/fpdf_page_colors.cpp
  core/fpdfapi/fpdf_page/fpdf_page_doc.cpp
  core/fpdfapi/fpdf_page/fpdf_page_func.cpp
  core/fpdfapi/fpdf_page/fpdf_page_parser.cpp
  core/fpdfapi/fpdf_page/fpdf_page_parser_old.cpp
  core/fpdfapi/fpdf_parser/cfdf_document.cpp
  core/fpdfapi/fpdf_parser/cpdf_array.cpp
  core/fpdfapi/fpdf_parser/cpdf_boolean.cpp
  core/fpdfapi/fpdf_parser/cpdf_crypto_handler.cpp
  core/fpdfapi/fpdf_parser/cpdf_data_avail.cpp
  core/fpdfapi/fpdf_parser/cpdf_dictionary.cpp
  core/fpdfapi/fpdf_parser/cpdf_document.cpp
  core/fpdfapi/fpdf_parser/cpdf_hint_tables.cpp
  core/fpdfapi/fpdf_parser/cpdf_indirect_object_holder.cpp
  core/fpdfapi/fpdf_parser/cpdf_name.cpp
  core/fpdfapi/fpdf_parser/cpdf_null.cpp
  core/fpdfapi/fpdf_parser/cpdf_number.cpp
  core/fpdfapi/fpdf_parser/cpdf_object.cpp
  core/fpdfapi/fpdf_parser/cpdf_parser.cpp
  core/fpdfapi/fpdf_parser/cpdf_reference.cpp
  core/fpdfapi/fpdf_parser/cpdf_security_handler.cpp
  core/fpdfapi/fpdf_parser/cpdf_simple_parser.cpp
  core/fpdfapi/fpdf_parser/cpdf_stream.cpp
  core/fpdfapi/fpdf_parser/cpdf_stream_acc.cpp
  core/fpdfapi/fpdf_parser/cpdf_string.cpp
  core/fpdfapi/fpdf_parser/cpdf_syntax_parser.cpp
  core/fpdfapi/fpdf_parser/fpdf_parser_decode.cpp
  core/fpdfapi/fpdf_parser/fpdf_parser_utility.cpp
  core/fpdfapi/fpdf_render/fpdf_render.cpp
  core/fpdfapi/fpdf_render/fpdf_render_cache.cpp
  core/fpdfapi/fpdf_render/fpdf_render_image.cpp
  core/fpdfapi/fpdf_render/fpdf_render_loadimage.cpp
  core/fpdfapi/fpdf_render/fpdf_render_pattern.cpp
  core/fpdfapi/fpdf_render/fpdf_render_text.cpp
  )

set(fpdfdoc_SRCS
  core/fpdfdoc/clines.cpp
  core/fpdfdoc/cpdf_variabletext.cpp
  core/fpdfdoc/cpvt_color.cpp
  core/fpdfdoc/cpvt_fontmap.cpp
  core/fpdfdoc/cpvt_generateap.cpp
  core/fpdfdoc/csection.cpp
  core/fpdfdoc/ctypeset.cpp
  core/fpdfdoc/doc_action.cpp
  core/fpdfdoc/doc_annot.cpp
  core/fpdfdoc/doc_basic.cpp
  core/fpdfdoc/doc_bookmark.cpp
  core/fpdfdoc/doc_form.cpp
  core/fpdfdoc/doc_formcontrol.cpp
  core/fpdfdoc/doc_formfield.cpp
  core/fpdfdoc/doc_link.cpp
  core/fpdfdoc/doc_metadata.cpp
  core/fpdfdoc/doc_ocg.cpp
  core/fpdfdoc/doc_tagged.cpp
  core/fpdfdoc/doc_utils.cpp
  core/fpdfdoc/doc_viewerPreferences.cpp
  core/fpdfdoc/doc_vt.cpp
  )

set(fpdfsdk_SRCS
  fpdfsdk/cfx_systemhandler.cpp
  fpdfsdk/fpdf_dataavail.cpp
  fpdfsdk/fpdf_ext.cpp
  fpdfsdk/fpdf_flatten.cpp
  fpdfsdk/fpdf_progressive.cpp
  fpdfsdk/fpdf_searchex.cpp
  fpdfsdk/fpdf_sysfontinfo.cpp
  fpdfsdk/fpdf_transformpage.cpp
  fpdfsdk/fpdfdoc.cpp
  fpdfsdk/fpdfeditimg.cpp
  fpdfsdk/fpdfeditpage.cpp
  fpdfsdk/fpdfformfill.cpp
  fpdfsdk/fpdfmissing.cpp
  fpdfsdk/fpdfppo.cpp
  fpdfsdk/fpdfsave.cpp
  fpdfsdk/fpdftext.cpp
  fpdfsdk/fpdfview.cpp
  fpdfsdk/fsdk_actionhandler.cpp
  fpdfsdk/fsdk_annothandler.cpp
  fpdfsdk/fsdk_baseannot.cpp
  fpdfsdk/fsdk_baseform.cpp
  fpdfsdk/fsdk_mgr.cpp
  fpdfsdk/fsdk_rendercontext.cpp
  )

set(fpdftext_SRCS
  core/fpdftext/fpdf_text_int.cpp
  core/fpdftext/unicodenormalizationdata.cpp
  )

set(fxcodec_SRCS
  core/fxcodec/codec/fx_codec.cpp
  core/fxcodec/codec/fx_codec_fax.cpp
  core/fxcodec/codec/fx_codec_flate.cpp
  core/fxcodec/codec/fx_codec_icc.cpp
  core/fxcodec/codec/fx_codec_jbig.cpp
  core/fxcodec/codec/fx_codec_jpeg.cpp
  core/fxcodec/codec/fx_codec_jpx_opj.cpp
  core/fxcodec/jbig2/JBig2_ArithDecoder.cpp
  core/fxcodec/jbig2/JBig2_ArithIntDecoder.cpp
  core/fxcodec/jbig2/JBig2_BitStream.cpp
  core/fxcodec/jbig2/JBig2_Context.cpp
  core/fxcodec/jbig2/JBig2_GrdProc.cpp
  core/fxcodec/jbig2/JBig2_GrrdProc.cpp
  core/fxcodec/jbig2/JBig2_GsidProc.cpp
  core/fxcodec/jbig2/JBig2_HtrdProc.cpp
  core/fxcodec/jbig2/JBig2_HuffmanDecoder.cpp
  core/fxcodec/jbig2/JBig2_HuffmanTable.cpp
  core/fxcodec/jbig2/JBig2_HuffmanTable_Standard.cpp
  core/fxcodec/jbig2/JBig2_Image.cpp
  core/fxcodec/jbig2/JBig2_PatternDict.cpp
  core/fxcodec/jbig2/JBig2_PddProc.cpp
  core/fxcodec/jbig2/JBig2_SddProc.cpp
  core/fxcodec/jbig2/JBig2_Segment.cpp
  core/fxcodec/jbig2/JBig2_SymbolDict.cpp
  core/fxcodec/jbig2/JBig2_TrdProc.cpp
  )

set(fxcrt_SRCS
  core/fxcrt/fx_basic_array.cpp
  core/fxcrt/fx_basic_bstring.cpp
  core/fxcrt/fx_basic_buffer.cpp
  core/fxcrt/fx_basic_coords.cpp
  core/fxcrt/fx_basic_gcc.cpp
  core/fxcrt/fx_basic_list.cpp
  core/fxcrt/fx_basic_memmgr.cpp
  core/fxcrt/fx_basic_plex.cpp
  core/fxcrt/fx_basic_utf.cpp
  core/fxcrt/fx_basic_util.cpp
  core/fxcrt/fx_basic_wstring.cpp
  core/fxcrt/fx_bidi.cpp
  core/fxcrt/fx_extension.cpp
  core/fxcrt/fx_ucddata.cpp
  core/fxcrt/fx_unicode.cpp
  core/fxcrt/fx_xml_composer.cpp
  core/fxcrt/fx_xml_parser.cpp
  core/fxcrt/fxcrt_posix.cpp
  core/fxcrt/fxcrt_stream.cpp
  core/fxcrt/fxcrt_windows.cpp
  )

set(fxedit_SRCS
  fpdfsdk/fxedit/fxet_ap.cpp
  fpdfsdk/fxedit/fxet_edit.cpp
  fpdfsdk/fxedit/fxet_list.cpp
  fpdfsdk/fxedit/fxet_module.cpp
  fpdfsdk/fxedit/fxet_pageobjs.cpp
  )

set(fxge_SRCS
  core/fxge/agg/fx_agg_driver.cpp
  core/fxge/dib/fx_dib_composite.cpp
  core/fxge/dib/fx_dib_convert.cpp
  core/fxge/dib/fx_dib_engine.cpp
  core/fxge/dib/fx_dib_main.cpp
  core/fxge/dib/fx_dib_transform.cpp
  core/fxge/fontdata/chromefontdata/FoxitDingbats.cpp
  core/fxge/fontdata/chromefontdata/FoxitFixed.cpp
  core/fxge/fontdata/chromefontdata/FoxitFixedBold.cpp
  core/fxge/fontdata/chromefontdata/FoxitFixedBoldItalic.cpp
  core/fxge/fontdata/chromefontdata/FoxitFixedItalic.cpp
  core/fxge/fontdata/chromefontdata/FoxitSans.cpp
  core/fxge/fontdata/chromefontdata/FoxitSansBold.cpp
  core/fxge/fontdata/chromefontdata/FoxitSansBoldItalic.cpp
  core/fxge/fontdata/chromefontdata/FoxitSansItalic.cpp
  core/fxge/fontdata/chromefontdata/FoxitSansMM.cpp
  core/fxge/fontdata/chromefontdata/FoxitSerif.cpp
  core/fxge/fontdata/chromefontdata/FoxitSerifBold.cpp
  core/fxge/fontdata/chromefontdata/FoxitSerifBoldItalic.cpp
  core/fxge/fontdata/chromefontdata/FoxitSerifItalic.cpp
  core/fxge/fontdata/chromefontdata/FoxitSerifMM.cpp
  core/fxge/fontdata/chromefontdata/FoxitSymbol.cpp
  core/fxge/freetype/fx_freetype.cpp
  core/fxge/ge/fx_ge.cpp
  core/fxge/ge/fx_ge_device.cpp
  core/fxge/ge/fx_ge_font.cpp
  core/fxge/ge/fx_ge_fontmap.cpp
  core/fxge/ge/fx_ge_path.cpp
  core/fxge/ge/fx_ge_text.cpp
  core/fxge/ifx_renderdevicedriver.cpp
  core/fxge/win32/fx_win32_device.cpp
  core/fxge/win32/fx_win32_dib.cpp
  core/fxge/win32/fx_win32_dwrite.cpp
  core/fxge/win32/fx_win32_gdipext.cpp
  core/fxge/win32/fx_win32_print.cpp
  )

set(javascript_SRCS
  fpdfsdk/javascript/JS_Runtime_Stub.cpp
  )

set(third_agg23_SRCS
  third_party/agg23/agg_curves.cpp
  third_party/agg23/agg_path_storage.cpp
  third_party/agg23/agg_rasterizer_scanline_aa.cpp
  third_party/agg23/agg_vcgen_dash.cpp
  third_party/agg23/agg_vcgen_stroke.cpp
  )

set(third_bigint_SRCS
  third_party/bigint/BigInteger.cc
  third_party/bigint/BigIntegerUtils.cc
  third_party/bigint/BigUnsigned.cc
  third_party/bigint/BigUnsignedInABase.cc
  )

set(third_lcms2_SRCS
  third_party/lcms2-2.6/src/cmscam02.c
  third_party/lcms2-2.6/src/cmscgats.c
  third_party/lcms2-2.6/src/cmscnvrt.c
  third_party/lcms2-2.6/src/cmserr.c
  third_party/lcms2-2.6/src/cmsgamma.c
  third_party/lcms2-2.6/src/cmsgmt.c
  third_party/lcms2-2.6/src/cmshalf.c
  third_party/lcms2-2.6/src/cmsintrp.c
  third_party/lcms2-2.6/src/cmsio0.c
  third_party/lcms2-2.6/src/cmsio1.c
  third_party/lcms2-2.6/src/cmslut.c
  third_party/lcms2-2.6/src/cmsmd5.c
  third_party/lcms2-2.6/src/cmsmtrx.c
  third_party/lcms2-2.6/src/cmsnamed.c
  third_party/lcms2-2.6/src/cmsopt.c
  third_party/lcms2-2.6/src/cmspack.c
  third_party/lcms2-2.6/src/cmspcs.c
  third_party/lcms2-2.6/src/cmsplugin.c
  third_party/lcms2-2.6/src/cmsps2.c
  third_party/lcms2-2.6/src/cmssamp.c
  third_party/lcms2-2.6/src/cmssm.c
  third_party/lcms2-2.6/src/cmstypes.c
  third_party/lcms2-2.6/src/cmsvirt.c
  third_party/lcms2-2.6/src/cmswtpnt.c
  third_party/lcms2-2.6/src/cmsxform.c
  )

set(third_openjpeg20_SRCS
  third_party/libopenjpeg20/bio.c
  third_party/libopenjpeg20/cio.c
  third_party/libopenjpeg20/dwt.c
  third_party/libopenjpeg20/event.c
  third_party/libopenjpeg20/function_list.c
  third_party/libopenjpeg20/image.c
  third_party/libopenjpeg20/invert.c
  third_party/libopenjpeg20/j2k.c
  third_party/libopenjpeg20/jp2.c
  third_party/libopenjpeg20/mct.c
  third_party/libopenjpeg20/mqc.c
  third_party/libopenjpeg20/openjpeg.c
  third_party/libopenjpeg20/opj_clock.c
  third_party/libopenjpeg20/pi.c
  third_party/libopenjpeg20/raw.c
  third_party/libopenjpeg20/t1.c
  third_party/libopenjpeg20/t2.c
  third_party/libopenjpeg20/tcd.c
  third_party/libopenjpeg20/tgt.c
  )

set(pdfwindow_SRCS
  fpdfsdk/pdfwindow/PWL_Button.cpp
  fpdfsdk/pdfwindow/PWL_Caret.cpp
  fpdfsdk/pdfwindow/PWL_ComboBox.cpp
  fpdfsdk/pdfwindow/PWL_Edit.cpp
  fpdfsdk/pdfwindow/PWL_EditCtrl.cpp
  fpdfsdk/pdfwindow/PWL_FontMap.cpp
  fpdfsdk/pdfwindow/PWL_Icon.cpp
  fpdfsdk/pdfwindow/PWL_ListBox.cpp
  fpdfsdk/pdfwindow/PWL_ScrollBar.cpp
  fpdfsdk/pdfwindow/PWL_SpecialButton.cpp
  fpdfsdk/pdfwindow/PWL_Utils.cpp
  fpdfsdk/pdfwindow/PWL_Wnd.cpp
  )

### Target ###################################################################

add_library(PDFium SHARED
  ${fdrm_SRCS}     # 2
  ${formfiller_SRCS}
  ${fpdfapi_SRCS}  # 5
  ${fpdfdoc_SRCS}  # 6
  ${fpdfsdk_SRCS}
  ${fpdftext_SRCS} # 7
  ${fxcodec_SRCS}  # 3
  ${fxcrt_SRCS}    # 1
  ${fxedit_SRCS}
  ${fxge_SRCS}     # 4
  ${javascript_SRCS}
  ${third_agg23_SRCS}
  ${third_bigint_SRCS}
  ${third_lcms2_SRCS}
  ${third_openjpeg20_SRCS}
  ${pdfwindow_SRCS}
  )

format_output_name(PDFium "PDFium")

target_compile_options(PDFium
  PRIVATE /wd4244 /wd4267 /wd4996
  )

target_compile_definitions(PDFium
  PRIVATE FPDFSDK_EXPORTS NOMINMAX OPJ_STATIC
  )

target_include_directories(PDFium
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE ${ft2_INCLUDES}
  PRIVATE ${jt_INCLUDES}
  PRIVATE ${zlib_INCLUDES}
  )

target_link_libraries(PDFium
  freetype jpeg-static zlib
  )

install(TARGETS PDFium
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  )

install(DIRECTORY
  public/
  DESTINATION include/fpdfsdk
  )
