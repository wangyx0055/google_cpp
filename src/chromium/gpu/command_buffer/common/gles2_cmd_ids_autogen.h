// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file is auto-generated from
// gpu/command_buffer/build_gles2_cmd_buffer.py
// It's formatted by clang-format using chromium coding style:
//    clang-format -i -style=chromium filename
// DO NOT EDIT!

#ifndef GPU_COMMAND_BUFFER_COMMON_GLES2_CMD_IDS_AUTOGEN_H_
#define GPU_COMMAND_BUFFER_COMMON_GLES2_CMD_IDS_AUTOGEN_H_

#define GLES2_COMMAND_LIST(OP)                           \
  OP(ActiveTexture)                            /* 256 */ \
  OP(AttachShader)                             /* 257 */ \
  OP(BindAttribLocationBucket)                 /* 258 */ \
  OP(BindBuffer)                               /* 259 */ \
  OP(BindBufferBase)                           /* 260 */ \
  OP(BindFramebuffer)                          /* 261 */ \
  OP(BindRenderbuffer)                         /* 262 */ \
  OP(BindSampler)                              /* 263 */ \
  OP(BindTexture)                              /* 264 */ \
  OP(BindTransformFeedback)                    /* 265 */ \
  OP(BlendColor)                               /* 266 */ \
  OP(BlendEquation)                            /* 267 */ \
  OP(BlendEquationSeparate)                    /* 268 */ \
  OP(BlendFunc)                                /* 269 */ \
  OP(BlendFuncSeparate)                        /* 270 */ \
  OP(BufferData)                               /* 271 */ \
  OP(BufferSubData)                            /* 272 */ \
  OP(CheckFramebufferStatus)                   /* 273 */ \
  OP(Clear)                                    /* 274 */ \
  OP(ClearColor)                               /* 275 */ \
  OP(ClearDepthf)                              /* 276 */ \
  OP(ClearStencil)                             /* 277 */ \
  OP(ColorMask)                                /* 278 */ \
  OP(CompileShader)                            /* 279 */ \
  OP(CompressedTexImage2DBucket)               /* 280 */ \
  OP(CompressedTexImage2D)                     /* 281 */ \
  OP(CompressedTexSubImage2DBucket)            /* 282 */ \
  OP(CompressedTexSubImage2D)                  /* 283 */ \
  OP(CopyBufferSubData)                        /* 284 */ \
  OP(CopyTexImage2D)                           /* 285 */ \
  OP(CopyTexSubImage2D)                        /* 286 */ \
  OP(CreateProgram)                            /* 287 */ \
  OP(CreateShader)                             /* 288 */ \
  OP(CullFace)                                 /* 289 */ \
  OP(DeleteBuffersImmediate)                   /* 290 */ \
  OP(DeleteFramebuffersImmediate)              /* 291 */ \
  OP(DeleteProgram)                            /* 292 */ \
  OP(DeleteRenderbuffersImmediate)             /* 293 */ \
  OP(DeleteSamplersImmediate)                  /* 294 */ \
  OP(DeleteShader)                             /* 295 */ \
  OP(DeleteTexturesImmediate)                  /* 296 */ \
  OP(DeleteTransformFeedbacksImmediate)        /* 297 */ \
  OP(DepthFunc)                                /* 298 */ \
  OP(DepthMask)                                /* 299 */ \
  OP(DepthRangef)                              /* 300 */ \
  OP(DetachShader)                             /* 301 */ \
  OP(Disable)                                  /* 302 */ \
  OP(DisableVertexAttribArray)                 /* 303 */ \
  OP(DrawArrays)                               /* 304 */ \
  OP(DrawElements)                             /* 305 */ \
  OP(Enable)                                   /* 306 */ \
  OP(EnableVertexAttribArray)                  /* 307 */ \
  OP(Finish)                                   /* 308 */ \
  OP(Flush)                                    /* 309 */ \
  OP(FramebufferRenderbuffer)                  /* 310 */ \
  OP(FramebufferTexture2D)                     /* 311 */ \
  OP(FramebufferTextureLayer)                  /* 312 */ \
  OP(FrontFace)                                /* 313 */ \
  OP(GenBuffersImmediate)                      /* 314 */ \
  OP(GenerateMipmap)                           /* 315 */ \
  OP(GenFramebuffersImmediate)                 /* 316 */ \
  OP(GenRenderbuffersImmediate)                /* 317 */ \
  OP(GenSamplersImmediate)                     /* 318 */ \
  OP(GenTexturesImmediate)                     /* 319 */ \
  OP(GenTransformFeedbacksImmediate)           /* 320 */ \
  OP(GetActiveAttrib)                          /* 321 */ \
  OP(GetActiveUniform)                         /* 322 */ \
  OP(GetAttachedShaders)                       /* 323 */ \
  OP(GetAttribLocation)                        /* 324 */ \
  OP(GetBooleanv)                              /* 325 */ \
  OP(GetBufferParameteriv)                     /* 326 */ \
  OP(GetError)                                 /* 327 */ \
  OP(GetFloatv)                                /* 328 */ \
  OP(GetFramebufferAttachmentParameteriv)      /* 329 */ \
  OP(GetIntegerv)                              /* 330 */ \
  OP(GetInternalformativ)                      /* 331 */ \
  OP(GetProgramiv)                             /* 332 */ \
  OP(GetProgramInfoLog)                        /* 333 */ \
  OP(GetRenderbufferParameteriv)               /* 334 */ \
  OP(GetSamplerParameterfv)                    /* 335 */ \
  OP(GetSamplerParameteriv)                    /* 336 */ \
  OP(GetShaderiv)                              /* 337 */ \
  OP(GetShaderInfoLog)                         /* 338 */ \
  OP(GetShaderPrecisionFormat)                 /* 339 */ \
  OP(GetShaderSource)                          /* 340 */ \
  OP(GetString)                                /* 341 */ \
  OP(GetTexParameterfv)                        /* 342 */ \
  OP(GetTexParameteriv)                        /* 343 */ \
  OP(GetUniformfv)                             /* 344 */ \
  OP(GetUniformiv)                             /* 345 */ \
  OP(GetUniformLocation)                       /* 346 */ \
  OP(GetVertexAttribfv)                        /* 347 */ \
  OP(GetVertexAttribiv)                        /* 348 */ \
  OP(GetVertexAttribPointerv)                  /* 349 */ \
  OP(Hint)                                     /* 350 */ \
  OP(InvalidateFramebufferImmediate)           /* 351 */ \
  OP(InvalidateSubFramebufferImmediate)        /* 352 */ \
  OP(IsBuffer)                                 /* 353 */ \
  OP(IsEnabled)                                /* 354 */ \
  OP(IsFramebuffer)                            /* 355 */ \
  OP(IsProgram)                                /* 356 */ \
  OP(IsRenderbuffer)                           /* 357 */ \
  OP(IsSampler)                                /* 358 */ \
  OP(IsShader)                                 /* 359 */ \
  OP(IsTexture)                                /* 360 */ \
  OP(IsTransformFeedback)                      /* 361 */ \
  OP(LineWidth)                                /* 362 */ \
  OP(LinkProgram)                              /* 363 */ \
  OP(PauseTransformFeedback)                   /* 364 */ \
  OP(PixelStorei)                              /* 365 */ \
  OP(PolygonOffset)                            /* 366 */ \
  OP(ReadBuffer)                               /* 367 */ \
  OP(ReadPixels)                               /* 368 */ \
  OP(ReleaseShaderCompiler)                    /* 369 */ \
  OP(RenderbufferStorage)                      /* 370 */ \
  OP(ResumeTransformFeedback)                  /* 371 */ \
  OP(SampleCoverage)                           /* 372 */ \
  OP(SamplerParameterf)                        /* 373 */ \
  OP(SamplerParameterfvImmediate)              /* 374 */ \
  OP(SamplerParameteri)                        /* 375 */ \
  OP(SamplerParameterivImmediate)              /* 376 */ \
  OP(Scissor)                                  /* 377 */ \
  OP(ShaderBinary)                             /* 378 */ \
  OP(ShaderSourceBucket)                       /* 379 */ \
  OP(StencilFunc)                              /* 380 */ \
  OP(StencilFuncSeparate)                      /* 381 */ \
  OP(StencilMask)                              /* 382 */ \
  OP(StencilMaskSeparate)                      /* 383 */ \
  OP(StencilOp)                                /* 384 */ \
  OP(StencilOpSeparate)                        /* 385 */ \
  OP(TexImage2D)                               /* 386 */ \
  OP(TexImage3D)                               /* 387 */ \
  OP(TexParameterf)                            /* 388 */ \
  OP(TexParameterfvImmediate)                  /* 389 */ \
  OP(TexParameteri)                            /* 390 */ \
  OP(TexParameterivImmediate)                  /* 391 */ \
  OP(TexStorage3D)                             /* 392 */ \
  OP(TexSubImage2D)                            /* 393 */ \
  OP(TexSubImage3D)                            /* 394 */ \
  OP(Uniform1f)                                /* 395 */ \
  OP(Uniform1fvImmediate)                      /* 396 */ \
  OP(Uniform1i)                                /* 397 */ \
  OP(Uniform1ivImmediate)                      /* 398 */ \
  OP(Uniform1ui)                               /* 399 */ \
  OP(Uniform1uivImmediate)                     /* 400 */ \
  OP(Uniform2f)                                /* 401 */ \
  OP(Uniform2fvImmediate)                      /* 402 */ \
  OP(Uniform2i)                                /* 403 */ \
  OP(Uniform2ivImmediate)                      /* 404 */ \
  OP(Uniform2ui)                               /* 405 */ \
  OP(Uniform2uivImmediate)                     /* 406 */ \
  OP(Uniform3f)                                /* 407 */ \
  OP(Uniform3fvImmediate)                      /* 408 */ \
  OP(Uniform3i)                                /* 409 */ \
  OP(Uniform3ivImmediate)                      /* 410 */ \
  OP(Uniform3ui)                               /* 411 */ \
  OP(Uniform3uivImmediate)                     /* 412 */ \
  OP(Uniform4f)                                /* 413 */ \
  OP(Uniform4fvImmediate)                      /* 414 */ \
  OP(Uniform4i)                                /* 415 */ \
  OP(Uniform4ivImmediate)                      /* 416 */ \
  OP(Uniform4ui)                               /* 417 */ \
  OP(Uniform4uivImmediate)                     /* 418 */ \
  OP(UniformMatrix2fvImmediate)                /* 419 */ \
  OP(UniformMatrix2x3fvImmediate)              /* 420 */ \
  OP(UniformMatrix2x4fvImmediate)              /* 421 */ \
  OP(UniformMatrix3fvImmediate)                /* 422 */ \
  OP(UniformMatrix3x2fvImmediate)              /* 423 */ \
  OP(UniformMatrix3x4fvImmediate)              /* 424 */ \
  OP(UniformMatrix4fvImmediate)                /* 425 */ \
  OP(UniformMatrix4x2fvImmediate)              /* 426 */ \
  OP(UniformMatrix4x3fvImmediate)              /* 427 */ \
  OP(UseProgram)                               /* 428 */ \
  OP(ValidateProgram)                          /* 429 */ \
  OP(VertexAttrib1f)                           /* 430 */ \
  OP(VertexAttrib1fvImmediate)                 /* 431 */ \
  OP(VertexAttrib2f)                           /* 432 */ \
  OP(VertexAttrib2fvImmediate)                 /* 433 */ \
  OP(VertexAttrib3f)                           /* 434 */ \
  OP(VertexAttrib3fvImmediate)                 /* 435 */ \
  OP(VertexAttrib4f)                           /* 436 */ \
  OP(VertexAttrib4fvImmediate)                 /* 437 */ \
  OP(VertexAttribI4i)                          /* 438 */ \
  OP(VertexAttribI4ivImmediate)                /* 439 */ \
  OP(VertexAttribI4ui)                         /* 440 */ \
  OP(VertexAttribI4uivImmediate)               /* 441 */ \
  OP(VertexAttribIPointer)                     /* 442 */ \
  OP(VertexAttribPointer)                      /* 443 */ \
  OP(Viewport)                                 /* 444 */ \
  OP(BlitFramebufferCHROMIUM)                  /* 445 */ \
  OP(RenderbufferStorageMultisampleCHROMIUM)   /* 446 */ \
  OP(RenderbufferStorageMultisampleEXT)        /* 447 */ \
  OP(FramebufferTexture2DMultisampleEXT)       /* 448 */ \
  OP(TexStorage2DEXT)                          /* 449 */ \
  OP(GenQueriesEXTImmediate)                   /* 450 */ \
  OP(DeleteQueriesEXTImmediate)                /* 451 */ \
  OP(BeginQueryEXT)                            /* 452 */ \
  OP(BeginTransformFeedback)                   /* 453 */ \
  OP(EndQueryEXT)                              /* 454 */ \
  OP(EndTransformFeedback)                     /* 455 */ \
  OP(InsertEventMarkerEXT)                     /* 456 */ \
  OP(PushGroupMarkerEXT)                       /* 457 */ \
  OP(PopGroupMarkerEXT)                        /* 458 */ \
  OP(GenVertexArraysOESImmediate)              /* 459 */ \
  OP(DeleteVertexArraysOESImmediate)           /* 460 */ \
  OP(IsVertexArrayOES)                         /* 461 */ \
  OP(BindVertexArrayOES)                       /* 462 */ \
  OP(SwapBuffers)                              /* 463 */ \
  OP(GetMaxValueInBufferCHROMIUM)              /* 464 */ \
  OP(EnableFeatureCHROMIUM)                    /* 465 */ \
  OP(ResizeCHROMIUM)                           /* 466 */ \
  OP(GetRequestableExtensionsCHROMIUM)         /* 467 */ \
  OP(RequestExtensionCHROMIUM)                 /* 468 */ \
  OP(GetProgramInfoCHROMIUM)                   /* 469 */ \
  OP(GetTranslatedShaderSourceANGLE)           /* 470 */ \
  OP(PostSubBufferCHROMIUM)                    /* 471 */ \
  OP(TexImageIOSurface2DCHROMIUM)              /* 472 */ \
  OP(CopyTextureCHROMIUM)                      /* 473 */ \
  OP(DrawArraysInstancedANGLE)                 /* 474 */ \
  OP(DrawElementsInstancedANGLE)               /* 475 */ \
  OP(VertexAttribDivisorANGLE)                 /* 476 */ \
  OP(GenMailboxCHROMIUM)                       /* 477 */ \
  OP(ProduceTextureCHROMIUMImmediate)          /* 478 */ \
  OP(ProduceTextureDirectCHROMIUMImmediate)    /* 479 */ \
  OP(ConsumeTextureCHROMIUMImmediate)          /* 480 */ \
  OP(CreateAndConsumeTextureCHROMIUMImmediate) /* 481 */ \
  OP(BindUniformLocationCHROMIUMBucket)        /* 482 */ \
  OP(GenValuebuffersCHROMIUMImmediate)         /* 483 */ \
  OP(DeleteValuebuffersCHROMIUMImmediate)      /* 484 */ \
  OP(IsValuebufferCHROMIUM)                    /* 485 */ \
  OP(BindValuebufferCHROMIUM)                  /* 486 */ \
  OP(SubscribeValueCHROMIUM)                   /* 487 */ \
  OP(PopulateSubscribedValuesCHROMIUM)         /* 488 */ \
  OP(UniformValuebufferCHROMIUM)               /* 489 */ \
  OP(BindTexImage2DCHROMIUM)                   /* 490 */ \
  OP(ReleaseTexImage2DCHROMIUM)                /* 491 */ \
  OP(TraceBeginCHROMIUM)                       /* 492 */ \
  OP(TraceEndCHROMIUM)                         /* 493 */ \
  OP(AsyncTexSubImage2DCHROMIUM)               /* 494 */ \
  OP(AsyncTexImage2DCHROMIUM)                  /* 495 */ \
  OP(WaitAsyncTexImage2DCHROMIUM)              /* 496 */ \
  OP(WaitAllAsyncTexImage2DCHROMIUM)           /* 497 */ \
  OP(DiscardFramebufferEXTImmediate)           /* 498 */ \
  OP(LoseContextCHROMIUM)                      /* 499 */ \
  OP(InsertSyncPointCHROMIUM)                  /* 500 */ \
  OP(WaitSyncPointCHROMIUM)                    /* 501 */ \
  OP(DrawBuffersEXTImmediate)                  /* 502 */ \
  OP(DiscardBackbufferCHROMIUM)                /* 503 */ \
  OP(ScheduleOverlayPlaneCHROMIUM)             /* 504 */ \
  OP(SwapInterval)                             /* 505 */ \
  OP(MatrixLoadfCHROMIUMImmediate)             /* 506 */ \
  OP(MatrixLoadIdentityCHROMIUM)               /* 507 */ \
  OP(BlendBarrierKHR)                          /* 508 */

enum CommandId {
  kStartPoint = cmd::kLastCommonId,  // All GLES2 commands start after this.
#define GLES2_CMD_OP(name) k##name,
  GLES2_COMMAND_LIST(GLES2_CMD_OP)
#undef GLES2_CMD_OP
      kNumCommands
};

#endif  // GPU_COMMAND_BUFFER_COMMON_GLES2_CMD_IDS_AUTOGEN_H_