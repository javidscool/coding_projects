package com.bignerdranch.android.geoquiz;

import android.app.Activity;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.ViewTreeObserver;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Arrays;

public class QuizActivity extends AppCompatActivity {

    private static final String TAG = "QuizActivity";

    private static final String KEY_INDEX = "index";
    private static final String KEY_DID_CHEAT = "cheat";
    private static final String KEY_ANSWER_CHECKED = "answer_checked";
    private static final String KEY_CHEAT_ARRAY = "cheatArray";
    private static final String KEY_CHEATS_LEFT = "cheatsLeft";
    private static final int REQUEST_CODE_CHEAT = 0;

    private Button mTrueButton;
    private Button mFalseButton;
    private ImageButton mNextButton;
    private ImageButton mPreviousButton;
    private Button mCheatButton;
    private TextView mQuestionTextView;
    private TextView mCheatsLeftTextView;

    private Question[] mQuestionBank = new Question[] {
            new Question(R.string.question_australia, true),
            new Question(R.string.question_oceans, true),
            new Question(R.string.question_mideast, false),
            new Question(R.string.question_africa, false),
            new Question(R.string.question_americas, true),
            new Question(R.string.question_asia, true)
    };

    private boolean[] mDidCheat = new boolean[mQuestionBank.length];

    private int mCurrentIndex = 0;
    private boolean mIsCheater = false;
    private boolean mAnswerChecked = false;

    private int mCheatsLeft = 3;

    //creating array to keep track of whether or not user answered question
    private Boolean[] initAnswers = new Boolean[mQuestionBank.length];
    ArrayList<Boolean> mAnsweredQuestion;

    //for keeping track of users score
    private int userScore = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate(Bundle) called");
        setContentView(R.layout.activity_quiz);

        //initializing answers array cause user hasn't answered any questions yet
        Arrays.fill(initAnswers, false);
        mAnsweredQuestion = new ArrayList<>(Arrays.asList(initAnswers));

        if(savedInstanceState != null){
            mCurrentIndex = savedInstanceState.getInt(KEY_INDEX, 0);
            mIsCheater = savedInstanceState.getBoolean(KEY_DID_CHEAT, false);
            mAnswerChecked = savedInstanceState.getBoolean(KEY_ANSWER_CHECKED, false);
            mDidCheat = savedInstanceState.getBooleanArray(KEY_CHEAT_ARRAY);
            mCheatsLeft = savedInstanceState.getInt(KEY_CHEATS_LEFT, 0);
        }

        mQuestionTextView = (TextView)findViewById(R.id.question_text_view);
        mQuestionTextView.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                mCurrentIndex = (mCurrentIndex + 1) % mQuestionBank.length;
                updateQuestion(true);
            }
        });

        mTrueButton = (Button)findViewById(R.id.true_button);
        mFalseButton = (Button)findViewById(R.id.false_button);

        mTrueButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                checkAnswer(true);
                mTrueButton.setEnabled(false);
                mFalseButton.setEnabled(false);
                mCheatButton.setEnabled(false);
            }
        });

        mFalseButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                checkAnswer(false);
                mTrueButton.setEnabled(false);
                mFalseButton.setEnabled(false);
                mCheatButton.setEnabled(false);
            }
        });

        mPreviousButton = (ImageButton)findViewById(R.id.previous_button);
        mPreviousButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                mCurrentIndex = (mCurrentIndex - 1) % mQuestionBank.length;

                if(mCurrentIndex < 0){
                    mCurrentIndex = mQuestionBank.length - 1;
                }

                updateQuestion(true);
            }
        });

        mNextButton = (ImageButton)findViewById(R.id.next_button);
        mNextButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                mCurrentIndex = (mCurrentIndex + 1) % mQuestionBank.length;

                updateQuestion(true);
            }
        });

        mCheatButton = (Button)findViewById(R.id.cheat_button);
        mCheatButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                //start CheatActivity
                boolean answerIsTrue = mQuestionBank[mCurrentIndex].isAnswerTrue();
                Intent i = CheatActivity.newIntent(QuizActivity.this, answerIsTrue);
                startActivityForResult(i, REQUEST_CODE_CHEAT);
            }
        });
        setCheatButton();

        mCheatsLeftTextView = (TextView)findViewById(R.id.cheats_left);
        mCheatsLeftTextView.setText("Cheats Left: " + mCheatsLeft);

        //disable cheat button if we've cheated already
        if(mIsCheater){
            mCheatButton.setEnabled(false);
        }

        //disable true and false button if we've already clicked on one of them
        //the view observer is on true button and we need check only this button
        //cause mAnswerChecked applies to both buttons
        ViewTreeObserver observer = mTrueButton.getViewTreeObserver();
        observer.addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener(){
            @Override
            public void onGlobalLayout(){
                if(mAnswerChecked){
                    mTrueButton.setEnabled(false);
                    mFalseButton.setEnabled(false);
                    mCheatButton.setEnabled(false);

                    //once we've set up the buttons, we don't need
                    //to call this listener anymore because our image view is created
                    mTrueButton.getViewTreeObserver().removeOnGlobalLayoutListener(this);
                }
            }
        });

        updateQuestion(false);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data){
        if(resultCode != Activity.RESULT_OK){
            return;
        }

        if(requestCode == REQUEST_CODE_CHEAT){
            if(data == null){
                return;
            }

            mIsCheater = CheatActivity.wasAnswerShown(data);
            mDidCheat[mCurrentIndex] = mIsCheater;
            //disable cheat button if we've cheated already
            if(mIsCheater){
                mCheatButton.setEnabled(false);

                if(mCheatsLeft > 0) {
                    mCheatsLeft--;
                    mCheatsLeftTextView.setText("Cheats Left: " + mCheatsLeft);
                }
            }
        }
    }


    @Override
    public void onStart(){
        super.onStart();
        Log.d(TAG, "onStart() called");
    }

    @Override
    public void onResume(){
        super.onResume();
        Log.d(TAG, "onResume() called");
    }

    @Override
    public void onPause(){
        super.onPause();
        Log.d(TAG, "onPause() called");
    }

    @Override
    public void onStop(){
        super.onStop();
        Log.d(TAG, "onStop() called");
    }

    @Override
    public void onDestroy(){
        super.onDestroy();
        Log.d(TAG, "onDestroy() called");
    }

    @Override
    public void onSaveInstanceState(Bundle savedInstanceState){
        super.onSaveInstanceState(savedInstanceState);
        Log.i(TAG, "onSaveInstanceState");
        savedInstanceState.putInt(KEY_INDEX, mCurrentIndex);
        savedInstanceState.putBoolean(KEY_DID_CHEAT, mIsCheater);
        savedInstanceState.putBoolean(KEY_ANSWER_CHECKED, mAnswerChecked);
        savedInstanceState.putBooleanArray(KEY_CHEAT_ARRAY, mDidCheat);
        savedInstanceState.putInt(KEY_CHEATS_LEFT, mCheatsLeft);
    }


    private void updateQuestion(boolean goToNext){
        int question = mQuestionBank[mCurrentIndex].getTextResId();
        mQuestionTextView.setText(question);

        if(goToNext){
            mTrueButton.setEnabled(true);
            mFalseButton.setEnabled(true);

            setCheatButton();
            mAnswerChecked = false;

            //reset cheater for next question
            mIsCheater = false;
        }
    }

    private void setCheatButton(){
        if(mCheatsLeft == 0){
            mCheatButton.setEnabled(false);
        }
        else {
            mCheatButton.setEnabled(true);
        }
    }

    private void checkAnswer(boolean userPressedTrue){
        //answer is checked
        mAnswerChecked = true;

        boolean answerIsTrue = mQuestionBank[mCurrentIndex].isAnswerTrue();

        int messageResId = 0;

        if(mIsCheater){
            messageResId = R.string.judgement_toast;
        }
        else if(mDidCheat[mCurrentIndex]){
            messageResId = R.string.judgement_toast;
        }
        else {
            if (userPressedTrue == answerIsTrue) {
                messageResId = R.string.correct_toast;

                if(mAnsweredQuestion.get(mCurrentIndex) == false){
                    userScore++;
                }
            } else {
                messageResId = R.string.incorrect_toast;
            }
        }

        //keep track of which questions user answered
        mAnsweredQuestion.set(mCurrentIndex, true);

        //if user answered all questions, display score
        if(!mAnsweredQuestion.contains(false)){
            int userPercentageScore = (int)((userScore * 100.0f) / mQuestionBank.length);

            Toast.makeText(this, "User score is: " + userPercentageScore + "%", Toast.LENGTH_SHORT).show();
        }

        Toast toast = Toast.makeText(this, messageResId, Toast.LENGTH_SHORT);
        toast.setGravity(Gravity.TOP, 0, 0);
        toast.show();
    }
}
